#pragma once

#include "FS.h"
#include "SPIFFS.h"
#include <ArduinoJson.h>

struct TtcServerConfiguration
{
    bool debugMode;

    int port = 80;
    size_t apiKeyLength = 50;
    int apiThrottleIntervalMs = 1000;
    int maxStoredUsers = 20;
    int maxApiKeysPerUser = 3;
    int maxApiKeysTotal = 60;

    bool selfHostMode = false;
};

class ServerConfiguration
{
private:
    TtcServerConfiguration config;

    String configFileName = "/serverConfig.json";
    size_t configFileMaxSize = 1024;

    const String flashConfigKeys[8] = {"debugMode", "port", "apiKeyLength", "apiThrottleIntervalMs", "maxStoredUsers", "maxApiKeysPerUser", "maxApiKeysTotal", "selfHostMode"};

public:
    ServerConfiguration(/* args */);
    ~ServerConfiguration();

    String getConfigFileName() const { return configFileName; }
    void setConfigFileName(const String &configFileName_) { configFileName = configFileName_; }

    size_t getConfigFileMaxSize() const { return configFileMaxSize; }
    void setConfigFileMaxSize(const size_t &configFileMaxSize_) { configFileMaxSize = configFileMaxSize_; }

    bool getDebugMode() const { return config.debugMode; }
    void setDebugMode(bool debugMode_) { config.debugMode = debugMode_; }

    int getPort() const { return config.port; }
    void setPort(int port_) { config.port = port_; }

    size_t getApiKeyLength() const { return config.apiKeyLength; }
    void setApiKeyLength(const size_t &apiKeyLength_) { config.apiKeyLength = apiKeyLength_; }

    int getApiThrottleIntervalMs() const { return config.apiThrottleIntervalMs; }
    void setApiThrottleIntervalMs(int apiThrottleIntervalMs_) { config.apiThrottleIntervalMs = apiThrottleIntervalMs_; }

    int getMaxStoredUsers() const { return config.maxStoredUsers; }
    void setMaxStoredUsers(int maxStoredUsers_) { config.maxStoredUsers = maxStoredUsers_; }

    int getMaxApiKeysPerUser() const { return config.maxApiKeysPerUser; }
    void setMaxApiKeysPerUser(int maxApiKeysPerUser_) { config.maxApiKeysPerUser = maxApiKeysPerUser_; }

    int getMaxApiKeysTotal() const { return config.maxApiKeysTotal; }
    void setMaxApiKeysTotal(int maxApiKeysTotal_) { config.maxApiKeysTotal = maxApiKeysTotal_; }

    bool getSelfHostMode() const { return config.selfHostMode; }
    void setSelfHostMode(bool selfHostMode_) { config.selfHostMode = selfHostMode_; }

    void printToSerial();
    DynamicJsonDocument printToJson();
    String printToSerializedPrettyJson();
    bool setFromJson(DynamicJsonDocument json);

    //  disk operations

    bool initFileSystem();
    bool loadFromDisk();
    bool saveToDisk();
};

ServerConfiguration::ServerConfiguration(/* args */)
{
}

ServerConfiguration::~ServerConfiguration()
{
}

void ServerConfiguration::printToSerial()
{
    Serial.println(getDebugMode() ? "\n Debug: ON" : "\n Debug: OFF");
    Serial.print((String) "\n Port: " + getPort() +
                 "\n API key length: " + getApiKeyLength() +
                 "\n API thottle interval (in mS): " + getApiThrottleIntervalMs() +
                 "\n Maximum # of stored users: " + getMaxStoredUsers() +
                 "\n Maximum # of API keys per user: " + getMaxApiKeysPerUser() +
                 "\n Maximum # of API keys in total: " + getMaxApiKeysTotal() +
                 "\n Is self host mode enabled?: " + getSelfHostMode());
}

DynamicJsonDocument ServerConfiguration::printToJson()
{
    DynamicJsonDocument json(configFileMaxSize);

    json["debugMode"] = getDebugMode();
    json["port"] = getPort();
    json["apiKeyLength"] = getApiKeyLength();
    json["apiThrottleIntervalMs"] = getApiThrottleIntervalMs();
    json["maxStoredUsers"] = getMaxStoredUsers();
    json["maxApiKeysPerUser"] = getMaxApiKeysPerUser();
    json["maxApiKeysTotal"] = getMaxApiKeysTotal();
    json["selfHostMode"] = getSelfHostMode();

    return json;
}

String ServerConfiguration::printToSerializedPrettyJson()
{
    DynamicJsonDocument json = printToJson();
    String serializedJson;

    serializeJsonPretty(json, serializedJson);

    json.clear();

    return serializedJson;
}

bool ServerConfiguration::setFromJson(DynamicJsonDocument json)
{
    for (String configKey : flashConfigKeys)
    {
        if (!json.containsKey(configKey))
        {
            if (Serial)
            {
                Serial.println("1, ERR CONF JSON KEYS, server configuration load error: Missing required configuration value: " + configKey);
            }
            return false;
        }
    }
    if (Serial)
    {
        Serial.println("0, OK CONF JSON, configuration json parsing successful.");
        Serial.println("Loaded configuration values in json:");
        serializeJsonPretty(json, Serial);
    }

    setDebugMode(json["debugMode"].as<bool>());
    setPort(json["port"].as<int>());
    setApiKeyLength(json["apiKeyLength"].as<size_t>());
    setApiThrottleIntervalMs(json["apiThrottleIntervalMs"].as<int>());
    setMaxStoredUsers(json["maxStoredUsers"].as<int>());
    setMaxApiKeysPerUser(json["maxApiKeysPerUser"].as<int>());
    setMaxApiKeysTotal(json["maxApiKeysTotal"].as<int>());
    setSelfHostMode(json["selfHostMode"].as<bool>());

    if (Serial)
    {
        Serial.println("0, OK CONF LOAD, server configuration successfully loaded.");

        Serial.println("Active configuration content:");
        printToSerial();
    }

    json.clear();

    return true;
}

//  TODO: these are common functionality in multiple config classes, extract these functions and inherit them.

bool ServerConfiguration::initFileSystem()
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

bool ServerConfiguration::loadFromDisk()
{
    File configFile = SPIFFS.open(configFileName, "r");
    if (!configFile)
    {
        Serial.println("1, ERR CONF LOAD FIO, server configuration load error: File IO error.");

        return false;
    }

    size_t configFileSize = configFile.size();
    if (configFileSize > configFileMaxSize)
    {
        Serial.println("1, ERR CONF LOAD SIZE, server configuration load error: File too large.");

        return false;
    }

    // alloc
    std::unique_ptr<char[]> buf(new char[configFileSize]);
    configFile.readBytes(buf.get(), configFileSize);

    if (Serial)
    {
        Serial.println("0, OK CONF LOAD, configuration loaded successfully.");
        Serial.println("Loaded configuration buffer content:");
        Serial.println(buf.get());
    }

    DynamicJsonDocument json(configFileMaxSize);

    DeserializationError error = deserializeJson(json, buf.get());

    if (error)
    {
        if (Serial)
        {
            Serial.println("1, ERR CONF LOAD JSON, server configuration load error: JSON parsing error.");
            Serial.println(error.c_str());
        }
        return false;
    }
    else
    {
        for (String configKey : flashConfigKeys)
        {
            if (!json.containsKey(configKey))
            {
                if (Serial)
                {
                    Serial.println(configKey + ": " + error.c_str());
                    Serial.println("1, ERR CONF JSON KEYS, server configuration load error: Missing required configuration value.");
                }
                return false;
            }
        }
        if (Serial)
        {
            Serial.println("0, OK CONF JSON, configuration json parsing successful.");
            Serial.println("Loaded configuration values in json:");
            serializeJsonPretty(json, Serial);
        }

        setDebugMode(json["debugMode"].as<bool>());
        setPort(json["port"].as<int>());
        setApiKeyLength(json["apiKeyLength"].as<size_t>());
        setApiThrottleIntervalMs(json["apiThrottleIntervalMs"].as<int>());
        setMaxStoredUsers(json["maxStoredUsers"].as<int>());
        setMaxApiKeysPerUser(json["maxApiKeysPerUser"].as<int>());
        setMaxApiKeysTotal(json["maxApiKeysTotal"].as<int>());
        setSelfHostMode(json["selfHostMode"].as<bool>());

        if (Serial)
        {
            Serial.println("0, OK CONF LOAD, server configuration successfully loaded.");

            Serial.println("Active configuration content:");
            printToSerial();
        }
    }

    json.clear();

    return true;
}

bool ServerConfiguration::saveToDisk()
{
    File configFile = SPIFFS.open(configFileName, "w");
    if (!configFile)
    {
        if (Serial)
        {
            Serial.println("1, ERR CONF WRITE FIO, Error when saving server configuration to disk: File IO error.");
        }
        return false;
    }

    int newFileSize = configFile.print(printToSerializedPrettyJson());

    if (newFileSize < 0)
    {
        if (Serial)
        {
            Serial.println("1, ERR CONF WRITE FILE WRITE, Error when saving server configuration to disk: File write error.");
        }
        return false;
    }
    else
    {
        if (Serial)
        {
            Serial.println((String) "0, OK CONF WRITE, server configuration saved to disk successfully, new filesize: " + newFileSize + " bites.");
        }
    }

    configFile.close();

    return true;
}