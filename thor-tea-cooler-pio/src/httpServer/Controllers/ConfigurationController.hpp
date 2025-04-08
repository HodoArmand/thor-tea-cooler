#pragma once

#include <Arduino.h>
#define CONFIG_ESP_HTTPS_SERVER_ENABLE
#define PSY_ENABLE_SSL true
#include <PsychicHttpsServer.h>

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

    esp_err_t getHardwareConfig(PsychicRequest *request_);
    esp_err_t setHardwareConfig(PsychicRequest *request_, JsonVariant &json);

    esp_err_t getNetworkConfig(PsychicRequest *request_);
    esp_err_t setNetworkConfig(PsychicRequest *request_, JsonVariant &json);

    esp_err_t getServerConfig(PsychicRequest *request_);
    esp_err_t setServerConfig(PsychicRequest *request_, JsonVariant &json);
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

inline esp_err_t ConfigurationController::getHardwareConfig(PsychicRequest *request_)
{
    ApiRequest request(request_);
    esp_err_t response;

    if (!request.validate())
    {
        response = validationErrorsResponse(request_, request.validationErrors);
    }
    else if (!auth->isApiKeyValid(request.getAuthApiKey()))
    {
        response = simpleUnauthorizedResponse(request_);
    }
    else
    {
        String hwConfigValues = hwConfig->printToSerializedPrettyJson();
        response = simpleResponse(request_, 200, "ok", hwConfigValues);
    }

    return response;
}

inline esp_err_t ConfigurationController::setHardwareConfig(PsychicRequest *request_, JsonVariant &json)
{
    SetHardwareConfigRequest request(request_, json);
    esp_err_t response;

    if (!request.validate())
    {
        response = validationErrorsResponse(request_, request.validationErrors);
    }
    else if (!auth->isApiKeyValid(request.getAuthApiKey()))
    {
        response = simpleUnauthorizedResponse(request_);
    }
    else
    {
        hwConfig->setFromJson(request.bodyToJson());
        if (!hwConfig->saveToDisk())
        {
            response = simpleResponse(request_, 500, "HardwareConfig save server error.", "The request was processed successfully, but there was a serverside error when saving it. The error is most likely a DB/Disk IO error.");
        }
        else
        {
            response = simpleCreatedResponse(request_);
        }
    }

    return response;
}

inline esp_err_t ConfigurationController::getNetworkConfig(PsychicRequest *request_)
{
    ApiRequest request(request_);
    esp_err_t response;

    if (!request.validate())
    {
        response = validationErrorsResponse(request_, request.validationErrors);
    }
    else if (!auth->isApiKeyValid(request.getAuthApiKey()))
    {
        response = simpleUnauthorizedResponse(request_);
    }
    else
    {
        String networkConfigValues = networkConfig->printToSerializedPrettyJson();
        response = simpleResponse(request_, 200, "ok", networkConfigValues);
    }

    return response;
}

inline esp_err_t ConfigurationController::setNetworkConfig(PsychicRequest *request_, JsonVariant &json)
{
    SetNetworkConfigRequest request(request_, json);
    esp_err_t response;

    if (!request.validate())
    {
        response = validationErrorsResponse(request_, request.validationErrors);
    }
    else if (!auth->isApiKeyValid(request.getAuthApiKey()))
    {
        response = simpleUnauthorizedResponse(request_);
    }
    else
    {
        networkConfig->setFromJson(request.bodyToJson());
        if (!networkConfig->saveToDisk())
        {
            response = simpleResponse(request_, 500, "NetworkConfig save server error.", "The request was processed successfully, but there was a serverside error when saving it. The error is most likely a DB/Disk IO error.");
        }
        else
        {
            response = simpleCreatedResponse(request_);
        }
    }

    return response;
}

inline esp_err_t ConfigurationController::getServerConfig(PsychicRequest *request_)
{
    ApiRequest request(request_);
    esp_err_t response;

    if (!request.validate())
    {
        response = validationErrorsResponse(request_, request.validationErrors);
    }
    else if (!auth->isApiKeyValid(request.getAuthApiKey()))
    {
        response = simpleUnauthorizedResponse(request_);
    }
    else
    {
        String serverConfigValues = serverConfig->printToSerializedPrettyJson();
        response = simpleResponse(request_, 200, "ok", serverConfigValues);
    }

    return response;
}

inline esp_err_t ConfigurationController::setServerConfig(PsychicRequest *request_, JsonVariant &json)
{
    SetServerConfigRequest request(request_, json);
    esp_err_t response;

    if (!request.validate())
    {
        response = validationErrorsResponse(request_, request.validationErrors);
    }
    else if (!auth->isApiKeyValid(request.getAuthApiKey()))
    {
        response = simpleUnauthorizedResponse(request_);
    }
    else
    {
        bool apikeyLengthChanged = serverConfig->getApiKeyLength() != request.getBodyParamValueByName("apiKeyLength").toInt();

        serverConfig->setFromJson(request.bodyToJson());
        if (!serverConfig->saveToDisk())
        {
            response = simpleResponse(request_, 500, "ServerConfig save server error.", "The request was processed successfully, but there was a serverside error when saving it. The error is most likely a DB/Disk IO error.");
        }
        else if (apikeyLengthChanged)
        {
            auth->clearApiKeys();
            response = simpleResponse(request_, 201, "ok, restarting", "Api key length has been changed, apikeys cleared. Device restarts in 10 seconds.");
            // TODO: delays are not allowed in the PsychicHTTP, move this to a fn and call it via pointer.
            delay(10000);
            ESP.restart();
        }
        else
        {
            response = simpleCreatedResponse(request_);
        }
    }

    return response;
}
