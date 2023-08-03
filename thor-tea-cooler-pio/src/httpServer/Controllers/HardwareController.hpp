#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include "httpServer/Controllers/Controller.hpp"
#include "httpServer/Authorization.hpp"
#include "hardware/ttcHardware.hpp"
#include "httpServer/requestValidators/ApiRequest.hpp"
#include "httpServer/requestValidators/hardware/switchRelayRequest.hpp"
#include "httpServer/requestValidators/hardware/setRelaysRequest.hpp"
#include "httpServer/requestValidators/hardware/setTargetTemperatureRequest.hpp"

class HardwareController : Controller
{
private:
    Authorization *auth;
    TtcHardware *hw;

public:
    HardwareController(Authorization *auth_, TtcHardware *hw_);
    ~HardwareController();

    void getHardwareState(AsyncWebServerRequest *request_);

    void switchRelay(AsyncWebServerRequest *request_);
    void setRelays(AsyncWebServerRequest *request_);
    void setModeManual(AsyncWebServerRequest *request_);
    void setModeAuto(AsyncWebServerRequest *request_);
    void setTargetTemperature(AsyncWebServerRequest *request_);
    void startAutoCooling(AsyncWebServerRequest *request_);
    void stopAutoCooling(AsyncWebServerRequest *request_);

    void restartMcu(AsyncWebServerRequest *request_);
};

HardwareController::HardwareController(Authorization *auth_, TtcHardware *hw_)
{
    auth = auth_;
    hw = hw_;
}

HardwareController::~HardwareController()
{
}

inline void HardwareController::getHardwareState(AsyncWebServerRequest *request_)
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
        simpleBigResponse(request_, 200, "ok", hw->getHardwareStateAsJsonString());
    }
}

void HardwareController::switchRelay(AsyncWebServerRequest *request_)
{
    SwitchRelayRequest request(request_);

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
        hw->switchRelay(request.getBodyParamValueByName("relay").toInt());
        simpleOkResponse(request_);
    }
}

inline void HardwareController::setRelays(AsyncWebServerRequest *request_)
{
    SetRelaysRequest request(request_);

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
        bool relay1Value, relay2Value = false;
        relay1Value = request.validator.stringtoBool(request.getBodyParamValueByName("relay1"));
        relay2Value = request.validator.stringtoBool(request.getBodyParamValueByName("relay2"));
        hw->setRelays(relay1Value, relay2Value);

        simpleCreatedResponse(request_);
    }
}

inline void HardwareController::setModeManual(AsyncWebServerRequest *request_)
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
        hw->setModeManual();
        simpleCreatedResponse(request_);
    }
}

inline void HardwareController::setModeAuto(AsyncWebServerRequest *request_)
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
        TtcHardwareMode mode = hw->getMode();
        if (mode != autoCooling)
        {
            hw->setModeAuto();
            simpleCreatedResponse(request_);
        }
        else
        {
            simpleBigResponse(request_, 500, "Cooling in progress.", "Can't switch to auto ready mode, when cooling is in progress. Finish the autoCooling or switch to manual mode first.");
        }
    }
}

inline void HardwareController::setTargetTemperature(AsyncWebServerRequest *request_)
{
    SetTargetTemperatureRequest request(request_);

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
        hw->setTargetTemperature(request.getBodyParamValueByName("targetTemperature").toFloat());
        simpleCreatedResponse(request_);
    }
}

inline void HardwareController::startAutoCooling(AsyncWebServerRequest *request_)
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
        TtcHardwareMode mode = hw->getMode();
        if (mode == autoReady)
        {
            hw->startCooling();
            simpleCreatedResponse(request_);
        }
        else
        {
            simpleBigResponse(request_, 500, "Not in autoReady mode.", "Can't start the automatic cooling progress. Switch to autoReady mode first.");
        }
    }
}

inline void HardwareController::stopAutoCooling(AsyncWebServerRequest *request_)
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
        TtcHardwareMode mode = hw->getMode();
        if (mode == autoCooling)
        {
            hw->stopCooling();
            simpleCreatedResponse(request_);
        }
        else
        {
            simpleBigResponse(request_, 500, "Not in autoCooling mode.", "Can't stop the automatic cooling progress. Switch to autoCooling mode first.");
        }
    }
}

inline void HardwareController::restartMcu(AsyncWebServerRequest *request_)
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

        simpleResponse(request_, 201, "ok", "Microcontroller is restarting in 10 seconds.");
        delay(10000);
        ESP.restart();
    }
}
