#pragma once

#include "httpServer/requestValidators/Request.hpp"

class SetHardwareConfigRequest : public Request
{
public:
    SetHardwareConfigRequest(AsyncWebServerRequest *request) : Request(request)
    {
        rules = {
            {"header", "apiHeader"},
            {"debugMode", "required|bool"},
            {"relayIoPin1", "required|integer|gpioPin"},
            {"relayIoPin2", "required|integer|gpioPin"},
            {"oneWireIoPin", "required|integer"},
            {"temperatureSensorOffsetCelsius", "required|float"},
            {"temperatureTargetDefault", "required|float|between:20.00&75.00"}};
    }
};