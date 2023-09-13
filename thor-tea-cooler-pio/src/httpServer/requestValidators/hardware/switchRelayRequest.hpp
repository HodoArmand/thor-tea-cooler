#pragma once

#include "httpServer/requestValidators/Request.hpp"
class SwitchRelayRequest : public Request
{
public:
    SwitchRelayRequest(AsyncWebServerRequest *request) : Request(request)
    {
        rules = {
            {"header", "apiHeader"},
            {"relay", "required|integer|inArray:1&2"}};
    }
};