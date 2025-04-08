#pragma once

#include <Arduino.h>
#define PSY_ENABLE_SSL true
#define CONFIG_ESP_HTTPS_SERVER_ENABLE
#include <PsychicHttpsServer.h>

#include "httpServer/Controllers/Controller.hpp"
#include "httpServer/Authorization.hpp"
#include "hardware/ttcHardware.hpp"
#include "httpServer/requestValidators/ApiRequest.hpp"
#include "httpServer/requestValidators/hardware/switchRelayRequest.hpp"
#include "httpServer/requestValidators/hardware/setRelaysRequest.hpp"
#include "httpServer/requestValidators/hardware/setTargetTemperatureRequest.hpp"
#include "httpServer/routing/ServerSideEventRouter.hpp"

class HardwareController : Controller
{
private:
    Authorization *auth;
    TtcHardware *hw;
    ServerSideEventRouter *sse;

public:
    HardwareController(Authorization *auth_, TtcHardware *hw_, ServerSideEventRouter *sse_);
    ~HardwareController();

    esp_err_t getHardwareState(PsychicRequest *request_);

    esp_err_t switchRelay(PsychicRequest *request_, JsonVariant &json);
    esp_err_t setRelays(PsychicRequest *request_, JsonVariant &json);
    esp_err_t setModeManual(PsychicRequest *request_);
    esp_err_t setModeAuto(PsychicRequest *request_);
    esp_err_t setTargetTemperature(PsychicRequest *request_, JsonVariant &json);
    esp_err_t startAutoCooling(PsychicRequest *request_);
    esp_err_t stopAutoCooling(PsychicRequest *request_);

    esp_err_t restartMcu(PsychicRequest *request_);
};

HardwareController::HardwareController(Authorization *auth_, TtcHardware *hw_, ServerSideEventRouter *sse_)
{
    auth = auth_;
    hw = hw_;
    sse = sse_;
}

HardwareController::~HardwareController()
{
}

inline esp_err_t HardwareController::getHardwareState(PsychicRequest *request_)
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
        response = simpleResponse(request_, 200, "ok", hw->getHardwareStateAsJsonString());
        sse->sendTeaState();
    }
    return response;
}

esp_err_t HardwareController::switchRelay(PsychicRequest *request_, JsonVariant &json)
{
    SwitchRelayRequest request(request_, json);
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
        hw->switchRelay(request.getBodyParamValueByName("relay").toInt());
        response = simpleCreatedResponse(request_);
        sse->sendTeaState();
    }

    return response;
}

inline esp_err_t HardwareController::setRelays(PsychicRequest *request_, JsonVariant &json)
{
    SetRelaysRequest request(request_, json);
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
        bool relay1Value, relay2Value = false;
        relay1Value = request.validator.stringToBool(request.getBodyParamValueByName("relay1"));
        relay2Value = request.validator.stringToBool(request.getBodyParamValueByName("relay2"));
        hw->setRelays(relay1Value, relay2Value);

        response = simpleCreatedResponse(request_);
        sse->sendTeaState();
    }
    return response;
}

inline esp_err_t HardwareController::setModeManual(PsychicRequest *request_)
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
        hw->setModeManual();
        response = simpleCreatedResponse(request_);
        sse->sendTeaState();
    }
    return response;
}

inline esp_err_t HardwareController::setModeAuto(PsychicRequest *request_)
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
        TtcHardwareMode mode = hw->getMode();
        if (mode != autoCooling)
        {
            hw->setModeAuto();
            response = simpleCreatedResponse(request_);
            sse->sendTeaState();
        }
        else
        {
            response = simpleResponse(request_, 500, "Cooling in progress.", "Can't switch to auto ready mode, when cooling is in progress. Finish the autoCooling or switch to manual mode first.");
            sse->sendTeaState();
        }
    }
    return response;
}

inline esp_err_t HardwareController::setTargetTemperature(PsychicRequest *request_, JsonVariant &json)
{
    SetTargetTemperatureRequest request(request_, json);
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
        hw->setTargetTemperature(request.getBodyParamValueByName("targetTemperature").toFloat());
        response = simpleCreatedResponse(request_);
        sse->sendTeaState();
    }

    return response;
}

inline esp_err_t HardwareController::startAutoCooling(PsychicRequest *request_)
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
        TtcHardwareMode mode = hw->getMode();
        if (mode == autoReady)
        {
            hw->startCooling();
            response = simpleCreatedResponse(request_);
            sse->sendTeaState();
        }
        else
        {
            response = simpleResponse(request_, 500, "Not in autoReady mode.", "Can't start the automatic cooling progress. Switch to autoReady mode first.");
            sse->sendTeaState();
        }
    }
    return response;
}

inline esp_err_t HardwareController::stopAutoCooling(PsychicRequest *request_)
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
        TtcHardwareMode mode = hw->getMode();
        if (mode == autoCooling)
        {
            hw->stopCooling();
            response = simpleCreatedResponse(request_);
            sse->sendTeaState();
        }
        else
        {
            response = simpleResponse(request_, 500, "Not in autoCooling mode.", "Can't stop the automatic cooling progress. Switch to autoCooling mode first.");
            sse->sendTeaState();
        }
    }
    return response;
}

inline esp_err_t HardwareController::restartMcu(PsychicRequest *request_)
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

        simpleResponse(request_, 201, "ok", "Microcontroller is restarting in 10 seconds.");
        delay(10000);
        ESP.restart();
        // TODO: delays are not allowed, move this to a dn.
    }
    return response;
}
