#pragma once

#include "httpServer/requestValidators/Request.hpp"

class SetNetworkConfigRequest : public Request
{
public:
    SetNetworkConfigRequest(PsychicRequest *request, JsonVariant &json) : Request(request, json)
    {
        rules = {
            {"header", "apiHeader"},
            {"debugMode", "required|bool"},
            {"ssid", "required"},
            {"password", "required"}};
    }
};