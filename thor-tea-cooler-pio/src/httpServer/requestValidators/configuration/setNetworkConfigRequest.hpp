#pragma once

#include "httpServer/requestValidators/Request.hpp"

class SetNetworkConfigRequest : public Request
{
public:
    SetNetworkConfigRequest(AsyncWebServerRequest *request) : Request(request)
    {
        rules = {
            {"header", "apiHeader"},
            {"debugMode", "required|bool"},
            {"ssid", "required"},
            {"password", "required"}};
    }
};