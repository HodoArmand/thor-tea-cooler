#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include "httpServer/Controllers/Controller.hpp"
#include "httpServer/Authorization.hpp"
#include "hardware/ttcHardware.hpp"
#include "httpServer/requestValidators/hardware/switchRelayRequest.hpp"

class HardwareController : Controller
{
private:
    Authorization *auth;
    TtcHardware *hw;

public:
    HardwareController(Authorization *auth_, TtcHardware *hw_);
    ~HardwareController();

    void switchRelay(AsyncWebServerRequest *request_);
};

HardwareController::HardwareController(Authorization *auth_, TtcHardware *hw_)
{
    auth = auth_;
    hw = hw_;
}

HardwareController::~HardwareController()
{
}

void HardwareController::switchRelay(AsyncWebServerRequest *request_)
{
    SwitchRelayRequest request(request_);

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
            hw->switchRelay(request.getBodyParamValueByName("relay").toInt());
            simpleOkResponse(request_);
        }
    }
}
