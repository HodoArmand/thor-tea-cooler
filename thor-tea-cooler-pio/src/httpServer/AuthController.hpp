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
    int getUserIndexById(int id_);
    int getUserIndexByName(String name_);

    bool loadApiKeysFromDisk();
    bool saveApiKeysToDisk();
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
        User tempUser;

        for (JsonObject userRow : json.as<JsonArray>())
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

bool AuthController::loadApiKeysFromDisk()
{

}

bool AuthController::saveApiKeysToDisk()
{
    
}