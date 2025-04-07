#pragma once

#include "httpServer/requestValidators/Request.hpp"

class SetTargetTemperatureRequest : public Request
{
public:
    SetTargetTemperatureRequest(PsychicRequest *request, JsonVariant &json) : Request(request, json)
    {
        rules = {
            {"header", "apiHeader"},
            {"targetTemperature", "required|float|between:20.00&75.00"}};
    }
};