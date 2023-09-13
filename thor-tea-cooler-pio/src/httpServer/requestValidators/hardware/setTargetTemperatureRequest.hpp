#pragma once

#include "httpServer/requestValidators/Request.hpp"

class SetTargetTemperatureRequest : public Request
{
public:
    SetTargetTemperatureRequest(AsyncWebServerRequest *request) : Request(request)
    {
        rules = {
            {"header", "apiHeader"},
            {"targetTemperature", "required|float|between:20.00&75.00"}};
    }
};