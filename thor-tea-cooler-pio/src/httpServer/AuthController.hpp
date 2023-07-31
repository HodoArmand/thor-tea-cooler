#include "serverConfiguration.hpp"
#include "User.hpp"

#include <iostream>
#include <vector>

using namespace std;

struct ApiKey
{
    int userId;
    String key;
};

enum RegisterUserResult
{
    REG_USR_OK,
    REG_USR_NAME_TAKEN,
    REG_USR_BAD_PW,
    REG_USR_DISK_ERR
};

class AuthController
{
private:
    bool debugMode;
    vector<ApiKey> apiKeys;

    size_t apiKeyLength = 50;
    int apiThrottleIntervalMs = 1000;
    int maxStoredUsers = 20;
    int maxApiKeysPerUser = 3;
    int maxApiKeysTotal = 60;

    int maxUserFileSize = 3072;
    int maxApiKeyFileSize = 2048;

public:
    AuthController(ServerConfiguration *config);
    ~AuthController();

    vector<User> users;

    bool getDebugMode() const { return debugMode; }
    void setDebugMode(bool debugMode_) { debugMode = debugMode_; }

    bool initFileSystem();

    DynamicJsonDocument printUsersToJson();
    String printUsersToSerializedPrettyJson();
    bool loadUsersFromDisk();
    bool saveUsersToDisk();

    User findUserById(int id_);
    User findUserByName(String name_);
    User findUserByApiKey(String apiKey_);
    int getUserIndexById(int id_);
    int getUserIndexByName(String name_);
    bool isUserNameUnique(String name_);
    bool isUserNameUnique(String name_, int userId_);
    bool isPasswordValid(String password_);

    DynamicJsonDocument printApiKeysToJson();
    String printApiKeysToSerializedPrettyJson();
    bool loadApiKeysFromDisk();
    bool saveApiKeysToDisk();

    String generateApiKey();
    bool isApiKeyUnique(String apiKey);
    void makeApiKeyUnique(int apiKeyIndex);
    bool isApiKeyValid(String apiKey_);
    bool clearApiKeys();

    RegisterUserResult registerUser(String userName, String password);
    String loginUser(String userName, String password);
    bool logoutUser(int userId);
    bool editUser(int userId, String userName, String password);
    bool editUser(int userId, String userName);
    bool editUser(int userId, String password);
    bool deleteUser(int userId);
};

AuthController::AuthController(ServerConfiguration *config)
{
    debugMode = config->getDebugMode();
    apiKeyLength = config->getApiKeyLength();
    apiThrottleIntervalMs = config->getApiThrottleIntervalMs();
    maxStoredUsers = config->getMaxStoredUsers();
    maxApiKeysPerUser = config->getMaxApiKeysPerUser();
    maxApiKeysTotal = config->getMaxApiKeysTotal();

    users.reserve(maxStoredUsers);
    maxUserFileSize = maxStoredUsers * 200;
    maxApiKeyFileSize = maxStoredUsers * 150;
}

AuthController::~AuthController()
{
}

bool AuthController::initFileSystem()
{
    if (!SPIFFS.begin())
    {
        if (Serial)
        {
            Serial.println("1, ERR FS INIT, Filesystem initialization error.");
        }
        return false;
    }
    else
    {
        if (Serial)
        {
            Serial.println("0, OK FS INIT, Filesystem initialization successful.");
        }
        return true;
    }
}

/*  TODO: add encryption to user pw data*/

bool AuthController::loadUsersFromDisk()
{
    File usersFile = SPIFFS.open("/users.json", "r");
    if (!usersFile)
    {
        Serial.println("1, ERR USER LOAD FIO, server users load error: File IO error.");

        return false;
    }

    // alloc
    unique_ptr<char[]> buf(new char[maxUserFileSize]);
    usersFile.readBytes(buf.get(), maxUserFileSize);

    if (Serial)
    {
        Serial.println("0, OK USER LOAD, users loaded successfully.");
        Serial.println("Loaded users buffer content:");
        Serial.println(buf.get());
    }

    DynamicJsonDocument json(maxUserFileSize);

    DeserializationError error = deserializeJson(json, buf.get());

    if (error)
    {
        if (Serial)
        {
            Serial.println("1, ERR USER USER JSON, users load error: JSON parsing error.");
            Serial.println(error.c_str());
        }
        return false;
    }
    else
    {
        JsonArray jsonTableOfUsers = json.as<JsonArray>();
        if (jsonTableOfUsers.size() > maxStoredUsers)
        {
            if (Serial)
            {
                Serial.println("1, ERR USER COUNT, users load error: Maximum stored users count exceeded..");
                Serial.println(error.c_str());
            }
            return false;
        }

        User tempUser;

        for (JsonObject userRow : jsonTableOfUsers)
        {
            tempUser.setId(userRow["id"].as<int>());
            tempUser.setName(userRow["name"].as<String>());
            tempUser.setPassword(userRow["password"].as<String>());
            users.push_back(tempUser);
        }

        users.shrink_to_fit();

        if (Serial)
        {
            Serial.println("0, OK USER JSON, users json parsing successful.");
            Serial.println("0, OK USER LOAD, users successfully loaded.");
        }
        if (Serial && debugMode)
        {
            Serial.println("Users loaded in RAM:");
            for (unsigned int i = 0; i < users.size(); i++)
            {
                Serial.println("ID: " + String(users[i].getId()) + " Name: " + users[i].getName() + " Password: " + users[i].getPassword());
            }
        }
    }

    json.clear();

    return true;
}

DynamicJsonDocument AuthController::printUsersToJson()
{
    DynamicJsonDocument json(maxUserFileSize);

    for (User user : users)
    {
        JsonObject userRow = json.createNestedObject();
        userRow["id"] = user.getId();
        userRow["name"] = user.getName();
        userRow["password"] = user.getPassword();
    }

    return json;
}

String AuthController::printUsersToSerializedPrettyJson()
{
    DynamicJsonDocument json = printUsersToJson();
    String serializedJson;

    serializeJsonPretty(json, serializedJson);

    json.clear();

    return serializedJson;
}

bool AuthController::saveUsersToDisk()
{
    File configFile = SPIFFS.open("/users.json", "w");
    if (!configFile)
    {
        if (Serial)
        {
            Serial.println("1, ERR USER WRITE FIO, Error when saving users to disk: File IO error.");
        }
        return false;
    }

    int newFileSize = configFile.print(printUsersToSerializedPrettyJson());

    if (newFileSize < 0)
    {
        if (Serial)
        {
            Serial.println("1, ERR USER WRITE FILE WRITE, Error when saving users to disk: File write error.");
        }
        return false;
    }
    else
    {
        if (Serial)
        {
            Serial.println((String) "0, OK USER WRITE, users saved to disk successfully, new filesize: " + newFileSize + " bites.");
        }
    }

    configFile.close();

    return true;
}

inline User AuthController::findUserById(int id_)
{
    for (User user : users)
    {
        if (user.getId() == id_)
        {
            return user;
        }
    }

    User userNotFound("userNotFound");

    return userNotFound;
}

inline User AuthController::findUserByName(String name_)
{
    for (User user : users)
    {
        if (user.getName() == name_)
        {
            return user;
        }
    }

    User userNotFound("userNotFound");

    return userNotFound;
}

inline User AuthController::findUserByApiKey(String apiKey_)
{
    int userId = -1;
    for (ApiKey apiKey : apiKeys)
    {
        if (apiKey.key == apiKey_)
        {
            userId = apiKey.userId;
            break;
        }
    }

    if (userId == -1)
    {
        User userNotFound("userNotFound");

        return userNotFound;
    }
    else
    {
        return users.at(getUserIndexById(userId));
    }
}

inline int AuthController::getUserIndexById(int id_)
{
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i].getId() == id_)
        {
            return i;
        }
    }

    return -1;
}

inline int AuthController::getUserIndexByName(String name_)
{
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i].getName() == name_)
        {
            return i;
        }
    }

    return -1;
}

inline bool AuthController::isUserNameUnique(String name_)
{
    for (User user : users)
    {
        if (user.getName() == name_)
        {
            return false;
        }
    }

    return true;
}

inline bool AuthController::isUserNameUnique(String name_, int userId_)
{
    for (User user : users)
    {
        if (user.getName() == name_ && user.getId() != userId_)
        {
            return false;
        }
    }

    return true;
}

inline bool AuthController::isPasswordValid(String password_)
{
    if (password_.length() >= 8 && password_.length() <= 32)
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline DynamicJsonDocument AuthController::printApiKeysToJson()
{
    DynamicJsonDocument json(maxApiKeyFileSize);

    for (ApiKey apiKey : apiKeys)
    {
        JsonObject apiKeyRow = json.createNestedObject();
        apiKeyRow["userId"] = apiKey.userId;
        apiKeyRow["key"] = apiKey.key;
    }

    return json;
}

inline String AuthController::printApiKeysToSerializedPrettyJson()
{
    DynamicJsonDocument json = printApiKeysToJson();
    String serializedJson;

    serializeJsonPretty(json, serializedJson);

    json.clear();

    return serializedJson;
}

bool AuthController::loadApiKeysFromDisk()
{
    File apiKeysFile = SPIFFS.open("/apiKeys.json", "r");
    if (!apiKeysFile)
    {
        Serial.println("1, ERR APIKEY LOAD FIO, server apiKeys load error: File IO error.");

        return false;
    }

    // alloc
    unique_ptr<char[]> buf(new char[maxApiKeyFileSize]);
    apiKeysFile.readBytes(buf.get(), maxApiKeyFileSize);

    if (Serial)
    {
        Serial.println("0, OK APIKEY LOAD, apiKeys loaded successfully.");
        Serial.println("Loaded apiKeys buffer content:");
        Serial.println(buf.get());
    }

    DynamicJsonDocument json(maxApiKeyFileSize);

    DeserializationError error = deserializeJson(json, buf.get());

    if (error)
    {
        if (Serial)
        {
            Serial.println("1, ERR APIKEY JSON, apiKeys load error: JSON parsing error.");
            Serial.println(error.c_str());
        }
        return false;
    }
    else
    {
        JsonArray jsonTableOfApiKeys = json.as<JsonArray>();
        if (jsonTableOfApiKeys.size() > maxApiKeysTotal)
        {
            if (Serial)
            {
                Serial.println("1, ERR APIKEY COUNT, users load error: Maximum stored apiKey count exceeded..");
                Serial.println(error.c_str());
            }
            return false;
        }

        ApiKey tempApiKey;

        for (JsonObject apiKeyRow : jsonTableOfApiKeys)
        {
            tempApiKey.userId = apiKeyRow["userId"].as<int>();
            tempApiKey.key = apiKeyRow["key"].as<String>();
            apiKeys.push_back(tempApiKey);
        }

        apiKeys.shrink_to_fit();

        if (Serial)
        {
            Serial.println("0, OK APIKEY JSON, apiKeys json parsing successful.");
            Serial.println("0, OK APIKEY LOAD, apiKeys successfully loaded.");
        }
        if (Serial && debugMode)
        {
            Serial.println("Api keys loaded in RAM:");
            for (ApiKey apiKey : apiKeys)
            {
                Serial.println("userId: " + String(apiKey.userId) + " API key: " + apiKey.key);
            }
        }
    }

    json.clear();

    return true;
}

bool AuthController::saveApiKeysToDisk()
{
    File apiKeysFile = SPIFFS.open("/apiKeys.json", "w");
    if (!apiKeysFile)
    {
        if (Serial)
        {
            Serial.println("1, ERR API KEY WRITE FIO, Error when saving apiKeys to disk: File IO error.");
        }
        return false;
    }

    int newFileSize = apiKeysFile.print(printApiKeysToSerializedPrettyJson());

    if (newFileSize < 0)
    {
        if (Serial)
        {
            Serial.println("1, ERR API KEY WRITE FILE WRITE, Error when saving apiKeys to disk: File write error.");
        }
        return false;
    }
    else
    {
        if (Serial)
        {
            Serial.println((String) "0, OK API KEY WRITE, apiKeys saved to disk successfully, new filesize: " + newFileSize + " bites.");
        }
    }

    apiKeysFile.close();

    return true;
}

String AuthController::generateApiKey()
{
    const char chars[35] = "123456789abcdefghijklmnopqrstuvwzy";
    String randomString = "";
    for (size_t i = 0; i < apiKeyLength; i++)
    {
        randomString += chars[random(0, 34)];
    }

    return randomString;
}

bool AuthController::isApiKeyUnique(String apiKey_)
{
    for (ApiKey apiKey : apiKeys)
    {
        if (apiKey.key == apiKey_)
        {
            return false;
        }
    }

    return true;
}

inline void AuthController::makeApiKeyUnique(int apiKeyIndex)
{
    if (!(apiKeyIndex >= apiKeys.size()))
    {
        apiKeys[apiKeyIndex].key[apiKeyLength - 1] = 'A';
    }
}

inline bool AuthController::isApiKeyValid(String apiKey_)
{
    for (ApiKey apiKey : apiKeys)
    {
        if (apiKey.key == apiKey_)
        {
            return true;
        }
    }

    return false;
}

inline bool AuthController::clearApiKeys()
{
    apiKeys.clear();
    apiKeys.shrink_to_fit();

    if (!saveApiKeysToDisk())
    {
        return false;
    }

    return true;
}

inline RegisterUserResult AuthController::registerUser(String userName, String password)
{
    int nextId = users.size();

    User newUser = User(nextId, userName, password);

    if (!isUserNameUnique(newUser.getName()))
    {
        return REG_USR_NAME_TAKEN;
    }

    if (!isPasswordValid(newUser.getPassword()))
    {
        return REG_USR_BAD_PW;
    }

    users.reserve(users.size() + 1);
    users.push_back(newUser);

    if (!saveUsersToDisk())
    {
        return REG_USR_DISK_ERR;
    }
    else
    {
        return REG_USR_OK;
    }
}

inline String AuthController::loginUser(String userName, String password)
{
    User loginUser(userName, password);
    int userId = -1;

    for (User user : users)
    {
        if (loginUser.getName() == user.getName() && loginUser.getPassword() == user.getPassword())
        {
            userId = user.getId();
            break;
        }
    }

    //  user not found/bad credentials
    if (userId == -1)
    {
        return "0";
    }

    for (ApiKey key : apiKeys)
    {
        if (key.userId == userId)
        {
            return key.key;
        }
    }

    ApiKey newApiKey;
    newApiKey.userId = userId;
    newApiKey.key = generateApiKey();
    apiKeys.reserve(apiKeys.size() + 1);
    apiKeys.push_back(newApiKey);
    if (!isApiKeyUnique(newApiKey.key))
    {
        makeApiKeyUnique(apiKeys.size() - 1);
    }

    if (!saveApiKeysToDisk())
    {
        return "ERROR: DISK IO";
    }

    return apiKeys.back().key;
}

inline bool AuthController::logoutUser(int userId)
{
    for (int i = 0; i < apiKeys.size(); i++)
    {
        if (apiKeys[i].userId == userId)
        {
            apiKeys.erase(apiKeys.begin() + i);
        }
    }

    if (!saveApiKeysToDisk())
    {
        return false;
    }

    return true;
}

inline bool AuthController::editUser(int userId, String userName, String password)
{
    User user = findUserById(userId);
    if (user.getId() == -1)
    {
        return false;
    }
    if (!isUserNameUnique(userName, user.getId()) || !isPasswordValid(password))
    {
        return false;
    }

    int userRamIndex = getUserIndexById(user.getId());

    users[userRamIndex].setName(userName);
    users[userRamIndex].setPassword(password);

    if (!saveUsersToDisk())
    {
        return false;
    }

    return true;
}

inline bool AuthController::editUser(int userId, String userName)
{
    User user = findUserById(userId);
    if (user.getId() == -1)
    {
        return false;
    }
    if (!isUserNameUnique(userName, user.getId()))
    {
        return false;
    }

    int userRamIndex = getUserIndexById(user.getId());

    users[userRamIndex].setName(userName);

    if (!saveUsersToDisk())
    {
        return false;
    }

    return true;
}

inline bool AuthController::deleteUser(int userId)
{
    if (users.size() <= 1)
    {
        return false;
    }

    User user = findUserById(userId);
    if (user.getId() == -1)
    {
        return false;
    }

    int userRamIndex = getUserIndexById(user.getId());

    users.erase(users.begin() + userRamIndex);
    users.shrink_to_fit();

    if (!saveUsersToDisk())
    {
        return false;
    }

    return true;
}
