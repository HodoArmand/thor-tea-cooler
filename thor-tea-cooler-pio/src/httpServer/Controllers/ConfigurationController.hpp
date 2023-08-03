#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include "hardware/hardwareConfiguration.hpp"
#include "network/networkConfiguration.hpp"
#include "httpServer/serverConfiguration.hpp"

#include "httpServer/Controllers/Controller.hpp"
#include "httpServer/Authorization.hpp"
#include "httpServer/requestValidators/ApiRequest.hpp"
#include "httpServer/requestValidators/configuration/setHardwareConfigRequest.hpp"
#include "httpServer/requestValidators/configuration/setNetworkConfigRequest.hpp"
#include "httpServer/requestValidators/configuration/setServerConfigRequest.hpp"

class ConfigurationController : Controller
{
private:
    Authorization *auth;
    HardwareConfiguration *hwConfig;
    NetworkConfiguration *networkConfig;
    ServerConfiguration *serverConfig;

public:
    ConfigurationController(Authorization *auth_, HardwareConfiguration *hwConfig_, NetworkConfiguration *networkConfig_, ServerConfiguration *serverConfig_);
    ~ConfigurationController();

    void getHardwareConfig(AsyncWebServerRequest *request_);
    void setHardwareConfig(AsyncWebServerRequest *request_);

    void getNetworkConfig(AsyncWebServerRequest *request_);
    void setNetworkConfig(AsyncWebServerRequest *request_);

    void getServerConfig(AsyncWebServerRequest *request_);
    void setServerConfig(AsyncWebServerRequest *request_);
};

ConfigurationController::ConfigurationController(Authorization *auth_, HardwareConfiguration *hwConfig_, NetworkConfiguration *networkConfig_, ServerConfiguration *serverConfig_)
{
    auth = auth_;
    hwConfig = hwConfig_;
    networkConfig = networkConfig_;
    serverConfig = serverConfig_;
}

ConfigurationController::~ConfigurationController()
{
}

inline void ConfigurationController::getHardwareConfig(AsyncWebServerRequest *request_)
{
    ApiRequest request(request_);

    if (!request.validate())
    {
        validationErrorsResponse(request_, request.validationErrors);
    }
    else if (!auth->isApiKeyValid(request.getAuthApiKey()))
    {
        simpleUnauthorizedResponse(request_);
    }
    else
    {
        String hwConfigValues = hwConfig->printToSerializedPrettyJson();
        simpleVeryBigResponse(request_, 200, "ok", hwConfigValues);
    }
}

inline void ConfigurationController::setHardwareConfig(AsyncWebServerRequest *request_)
{
    SetHardwareConfigRequest request(request_);

    if (!request.validate())
    {
        validationErrorsResponse(request_, request.validationErrors);
    }
    else if (!auth->isApiKeyValid(request.getAuthApiKey()))
    {
        //  TODO: refractor the auth check in controllers to elseif like this for better readability
        simpleUnauthorizedResponse(request_);
    }
    else
    {
        hwConfig->setFromJson(request.bodyToJson());
        if (!hwConfig->saveToDisk())
        {
            simpleResponse(request_, 500, "HardwareConfig save server error.", "The request was processed successfully, but there was a serverside error when saving it. The error is most likely a DB/Disk IO error.");
        }
        else
        {
            simpleCreatedResponse(request_);
        }
    }
}

inline void ConfigurationController::getNetworkConfig(AsyncWebServerRequest *request_)
{
    ApiRequest request(request_);

    if (!request.validate())
    {
        validationErrorsResponse(request_, request.validationErrors);
    }
    else
    {
        bool authorized = auth->isApiKeyValid(request.getAuthApiKey());

        if (!authorized)
        {
            simpleUnauthorizedResponse(request_);
        }
        else
        {
            String networkConfigValues = networkConfig->printToSerializedPrettyJson();
            simpleVeryBigResponse(request_, 200, "ok", networkConfigValues);
        }
    }
}

inline void ConfigurationController::setNetworkConfig(AsyncWebServerRequest *request_)
{
    SetNetworkConfigRequest request(request_);

    if (!request.validate())
    {
        validationErrorsResponse(request_, request.validationErrors);
    }
    else if (!auth->isApiKeyValid(request.getAuthApiKey()))
    {
        simpleUnauthorizedResponse(request_);
    }
    else
    {
        networkConfig->setFromJson(request.bodyToJson());
        if (!networkConfig->saveToDisk())
        {
            simpleResponse(request_, 500, "NetworkConfig save server error.", "The request was processed successfully, but there was a serverside error when saving it. The error is most likely a DB/Disk IO error.");
        }
        else
        {
            simpleCreatedResponse(request_);
        }
    }
}

inline void ConfigurationController::getServerConfig(AsyncWebServerRequest *request_)
{
    ApiRequest request(request_);

    if (!request.validate())
    {
        validationErrorsResponse(request_, request.validationErrors);
    }
    else
    {
        bool authorized = auth->isApiKeyValid(request.getAuthApiKey());

        if (!authorized)
        {
            simpleUnauthorizedResponse(request_);
        }
        else
        {
            String serverConfigValues = serverConfig->printToSerializedPrettyJson();
            simpleVeryBigResponse(request_, 200, "ok", serverConfigValues);
        }
    }
}

inline void ConfigurationController::setServerConfig(AsyncWebServerRequest *request_)
{
    SetServerConfigRequest request(request_);

    if (!request.validate())
    {
        validationErrorsResponse(request_, request.validationErrors);
    }
    else if (!auth->isApiKeyValid(request.getAuthApiKey()))
    {
        simpleUnauthorizedResponse(request_);
    }
    else
    {
        bool apikeyLengthChanged = serverConfig->getApiKeyLength() != request.getBodyParamValueByName("apiKeyLength").toInt();

        serverConfig->setFromJson(request.bodyToJson());
        if (!serverConfig->saveToDisk())
        {
            simpleResponse(request_, 500, "ServerConfig save server error.", "The request was processed successfully, but there was a serverside error when saving it. The error is most likely a DB/Disk IO error.");
        }
        else if (apikeyLengthChanged)
        {
            auth->clearApiKeys();
            simpleResponse(request_, 201, "ok, restarting", "Api key length has been changed, apikeys cleared. Device restarts in 10 seconds.");
            delay(10000);
            ESP.restart();
        }
        else
        {
            simpleCreatedResponse(request_);
        }
    }
}
