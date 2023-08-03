#pragma once

#include "FS.h"
#include "SPIFFS.h"
#include <ArduinoJson.h>

class Configuration
{
private:
    /* data */
public:
    Configuration(/* args */);
    ~Configuration();

    bool initFileSystem();

    DynamicJsonDocument printToJson();
    String printToSerializedPrettyJson();
};

Configuration::Configuration(/* args */)
{
}

Configuration::~Configuration()
{
}

bool Configuration::initFileSystem()
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

DynamicJsonDocument Configuration::printToJson()
{
    DynamicJsonDocument json(5);
    return json;
}

String Configuration::printToSerializedPrettyJson()
{
    DynamicJsonDocument json = printToJson();
    String serializedJson;

    serializeJsonPretty(json, serializedJson);

    json.clear();

    return serializedJson;
}
