#pragma once

#include "configuration/configuration.hpp"

struct TtcNetworkConfiguration
{
    bool debugMode;

    String ssid;
    String password;
};

class NetworkConfiguration : public Configuration
{
private:
    TtcNetworkConfiguration config;

    String configFileName = "/networkConfig.json";
    size_t configFileMaxSize = 256;

    const String flashConfigKeys[3] = {"debugMode", "ssid", "password"};

public:
    using Configuration::initFileSystem;
    using Configuration::printToSerializedPrettyJson;

    NetworkConfiguration();
    ~NetworkConfiguration();

    String getPassword() const { return config.password; }
    void setPassword(const String &password_) { config.password = password_; }

    String getSsid() const { return config.ssid; }
    void setSsid(const String &ssid_) { config.ssid = ssid_; }

    bool getDebugMode() const { return config.debugMode; }
    void setDebugMode(bool debugMode_) { config.debugMode = debugMode_; }

    size_t getConfigFileMaxSize() const { return configFileMaxSize; }
    void setConfigFileMaxSize(const size_t &configFileMaxSize_) { configFileMaxSize = configFileMaxSize_; }

    String getConfigFileName() const { return configFileName; }
    void setConfigFileName(const String &configFileName_) { configFileName = configFileName_; }

    void printToSerial();
    DynamicJsonDocument printToJson();
    String printToSerializedPrettyJson();
    bool setFromJson(DynamicJsonDocument json);

    //  disk operations

    bool loadFromDisk();
    bool saveToDisk();
};

NetworkConfiguration::NetworkConfiguration(/* args */
)
{
}

NetworkConfiguration::~NetworkConfiguration()
{
}

void NetworkConfiguration::printToSerial()
{
    Serial.println(getDebugMode() ? "\n Debug: ON" : "\n Debug: OFF");
    Serial.print((String) "\n SSID: " + getSsid() +
                 "\n Password: " + getPassword());
}

DynamicJsonDocument NetworkConfiguration::printToJson()
{
    DynamicJsonDocument json(configFileMaxSize);

    json["debugMode"] = getDebugMode();
    json["ssid"] = getSsid();
    json["password"] = getPassword();

    return json;
}

String NetworkConfiguration::printToSerializedPrettyJson()
{
    DynamicJsonDocument json = printToJson();
    String serializedJson;

    serializeJsonPretty(json, serializedJson);

    json.clear();

    return serializedJson;
}

bool NetworkConfiguration::setFromJson(DynamicJsonDocument json)
{
    for (String configKey : flashConfigKeys)
    {
        if (!json.containsKey(configKey))
        {
            if (Serial)
            {
                Serial.println("1, ERR CONF JSON KEYS, network configuration load error: Missing required configuration value: " + configKey);
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
    setSsid(json["ssid"].as<String>());
    setPassword(json["password"].as<String>());

    if (Serial)
    {
        Serial.println("0, OK CONF LOAD, network configuration successfully loaded.");

        Serial.println("Active configuration content:");
        printToSerial();
    }

    json.clear();

    return true;
}

bool NetworkConfiguration::loadFromDisk()
{
    File configFile = SPIFFS.open(configFileName, "r");
    if (!configFile)
    {
        Serial.println("1, ERR CONF LOAD FIO, network configuration load error: File IO error.");

        return false;
    }

    size_t configFileSize = configFile.size();
    if (configFileSize > configFileMaxSize)
    {
        Serial.println("1, ERR CONF LOAD SIZE, network configuration load error: File too large.");

        return false;
    }

    // alloc
    std::unique_ptr<char[]> buf(new char[configFileSize]);
    configFile.readBytes(buf.get(), configFileSize);

    if (Serial)
    {
        Serial.println("0, OK CONF LOAD, network configuration loaded successfully.");
        Serial.println("Loaded network configuration buffer content:");
        Serial.println(buf.get());
    }

    DynamicJsonDocument json(configFileMaxSize);

    DeserializationError error = deserializeJson(json, buf.get());

    if (error)
    {
        if (Serial)
        {
            Serial.println("1, ERR CONF LOAD JSON, network configuration load error: JSON parsing error.");
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
                    Serial.println("1, ERR CONF JSON KEYS, network configuration load error: Missing required configuration value.");
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
        setSsid(json["ssid"].as<String>());
        setPassword(json["password"].as<String>());

        if (Serial)
        {
            Serial.println("0, OK CONF LOAD, network configuration successfully loaded.");

            Serial.println("Active configuration content:");
            printToSerial();
        }
    }

    json.clear();

    return true;
}

bool NetworkConfiguration::saveToDisk()
{
    File configFile = SPIFFS.open(configFileName, "w");
    if (!configFile)
    {
        if (Serial)
        {
            Serial.println("1, ERR CONF WRITE FIO, Error when saving network configuration to disk: File IO error.");
        }
        return false;
    }

    int newFileSize = configFile.print(printToSerializedPrettyJson());

    if (newFileSize < 0)
    {
        if (Serial)
        {
            Serial.println("1, ERR CONF WRITE FILE WRITE, Error when saving network configuration to disk: File write error.");
        }
        return false;
    }
    else
    {
        if (Serial)
        {
            Serial.println((String) "0, OK CONF WRITE, network configuration saved to disk successfully, new filesize: " + newFileSize + " bites.");
        }
    }

    configFile.close();

    return true;
}
