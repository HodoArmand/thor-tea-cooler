#pragma once

#include "FS.h"
#include "SPIFFS.h"
#include <ArduinoJson.h>

struct TtcHardwareConfiguration
{
    bool debugMode;

    int relayIoPin1;
    int relayIoPin2;

    int oneWireIoPin;
    float temperatureSensorOffsetCelsius;

    float temperatureTargetDefault;
};

class HardwareConfiguration
{
private:
    //  props
    TtcHardwareConfiguration config;

    String configFileName = "/hardwareConfig.json";
    size_t configFileMaxSize = 2048;

    const String flashConfigKeys[6] = {"debugMode", "relayIoPin1", "relayIoPin2", "oneWireIoPin", "temperatureSensorOffsetCelsius", "temperatureTargetDefault"};

public:
    HardwareConfiguration();
    ~HardwareConfiguration();

    //  prop getter setters

    bool getDebugMode() const { return config.debugMode; }
    void setDebugMode(bool debugMode_) { config.debugMode = debugMode_; }

    int getRelayIoPin1() const { return config.relayIoPin1; }
    void setRelayIoPin1(int relayIoPin1_) { config.relayIoPin1 = relayIoPin1_; }

    int getRelayIoPin2() const { return config.relayIoPin2; }
    void setRelayIoPin2(int relayIoPin2_) { config.relayIoPin2 = relayIoPin2_; }

    int getOneWireIoPin() const { return config.oneWireIoPin; }
    void setOneWireIoPin(int oneWireIoPin_) { config.oneWireIoPin = oneWireIoPin_; }

    float getTemperatureSensorOffsetCelsius() const { return config.temperatureSensorOffsetCelsius; }
    void setTemperatureSensorOffsetCelsius(float temperatureSensorOffsetCelsius_) { config.temperatureSensorOffsetCelsius = temperatureSensorOffsetCelsius_; }

    float getTemperatureTargetDefault() const { return config.temperatureTargetDefault; }
    void setTemperatureTargetDefault(float temperatureTargetDefault_) { config.temperatureTargetDefault = temperatureTargetDefault_; }

    String getConfigFileName() const { return configFileName; }
    void setConfigFileName(const String &configFileName_) { configFileName = configFileName_; }

    size_t getConfigFileMaxSize() const { return configFileMaxSize; }
    void setConfigFileMaxSize(const size_t &configFileMaxSize_) { configFileMaxSize = configFileMaxSize_; }

    void printToSerial();
    DynamicJsonDocument printToJson();
    String printToSerializedPrettyJson();
    bool setFromJson(DynamicJsonDocument json);

    //  disk operations

    bool initFileSystem();
    bool loadFromDisk();
    bool saveToDisk();
};

HardwareConfiguration::HardwareConfiguration()
{
}

HardwareConfiguration::~HardwareConfiguration()
{
}

void HardwareConfiguration::printToSerial()
{
    Serial.println(getDebugMode() ? "\n Debug: ON" : "\n Debug: OFF");
    Serial.print((String) "\n REL1 GPIO Pin: " + getRelayIoPin1() +
                 "\n REL2 GPIO Pin: " + getRelayIoPin2() +
                 "\n OneWire GPIO Pin: " + getOneWireIoPin() +
                 "\n Temperature offset: " + getTemperatureSensorOffsetCelsius() +
                 "\n Default target temperature: " + getTemperatureTargetDefault());
}

DynamicJsonDocument HardwareConfiguration::printToJson()
{
    DynamicJsonDocument json(configFileMaxSize);

    json["debugMode"] = getDebugMode();
    json["relayIoPin1"] = getRelayIoPin1();
    json["relayIoPin2"] = getRelayIoPin2();
    json["oneWireIoPin"] = getOneWireIoPin();
    json["temperatureSensorOffsetCelsius"] = serialized(String(getTemperatureSensorOffsetCelsius(), 2));
    json["temperatureTargetDefault"] = serialized(String(getTemperatureTargetDefault(), 2));

    return json;
}

String HardwareConfiguration::printToSerializedPrettyJson()
{
    DynamicJsonDocument json = printToJson();
    String serializedJson;

    serializeJsonPretty(json, serializedJson);

    json.clear();

    return serializedJson;
}

inline bool HardwareConfiguration::setFromJson(DynamicJsonDocument json)
{
    for (String configKey : flashConfigKeys)
    {
        if (!json.containsKey(configKey))
        {
            if (Serial)
            {

                Serial.println("1, ERR CONF JSON KEYS, hardware configuration load error: Missing required configuration value: " + configKey);
            }
            return false;
        }
    }
    if (Serial)
    {
        Serial.println("0, OK CONF SET JSON, configuration json parsing successful.");
        Serial.println("Loaded configuration values in json:");
        serializeJsonPretty(json, Serial);
    }

    setDebugMode(json["debugMode"].as<bool>());
    setRelayIoPin1(json["relayIoPin1"].as<int>());
    setRelayIoPin2(json["relayIoPin2"].as<int>());
    setOneWireIoPin(json["oneWireIoPin"].as<int>());
    setTemperatureSensorOffsetCelsius(json["temperatureSensorOffsetCelsius"].as<float>());
    setTemperatureTargetDefault(json["temperatureTargetDefault"].as<float>());

    if (Serial)
    {
        Serial.println("0, OK CONF SET, hardware configuration successfully set.");

        Serial.println("Active configuration content:");
        printToSerial();
    }

    json.clear();

    return true;
}

//  TODO: these are common functionality in multiple config classes, extract these functions and inherit them.

bool HardwareConfiguration::initFileSystem()
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

bool HardwareConfiguration::loadFromDisk()
{
    File configFile = SPIFFS.open(configFileName, "r");
    if (!configFile)
    {
        Serial.println("1, ERR CONF LOAD FIO, hardware configuration load error: File IO error.");

        return false;
    }

    size_t configFileSize = configFile.size();
    if (configFileSize > configFileMaxSize)
    {
        Serial.println("1, ERR CONF LOAD SIZE, hardware configuration load error: File too large.");

        return false;
    }

    // alloc
    unique_ptr<char[]> buf(new char[configFileSize]);
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
            Serial.println("1, ERR CONF LOAD JSON, hardware configuration load error: JSON parsing error.");
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
                    Serial.println("1, ERR CONF JSON KEYS, hardware configuration load error: Missing required configuration value.");
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
        setRelayIoPin1(json["relayIoPin1"].as<int>());
        setRelayIoPin2(json["relayIoPin2"].as<int>());
        setOneWireIoPin(json["oneWireIoPin"].as<int>());
        setTemperatureSensorOffsetCelsius(json["temperatureSensorOffsetCelsius"].as<float>());
        setTemperatureTargetDefault(json["temperatureTargetDefault"].as<float>());

        if (Serial)
        {
            Serial.println("0, OK CONF LOAD, hardware configuration successfully loaded.");

            Serial.println("Active configuration content:");
            printToSerial();
        }
    }

    json.clear();

    return true;
}

bool HardwareConfiguration::saveToDisk()
{
    File configFile = SPIFFS.open(configFileName, "w");
    if (!configFile)
    {
        if (Serial)
        {
            Serial.println("1, ERR CONF WRITE FIO, Error when saving hardware configuration to disk: File IO error.");
        }
        return false;
    }

    int newFileSize = configFile.print(printToSerializedPrettyJson());

    if (newFileSize < 0)
    {
        if (Serial)
        {
            Serial.println("1, ERR CONF WRITE FILE WRITE, Error when saving hardware configuration to disk: File write error.");
        }
        return false;
    }
    else
    {
        if (Serial)
        {
            Serial.println((String) "0, OK CONF WRITE, hardware configuration saved to disk successfully, new filesize: " + newFileSize + " bites.");
        }
    }

    configFile.close();

    return true;
}