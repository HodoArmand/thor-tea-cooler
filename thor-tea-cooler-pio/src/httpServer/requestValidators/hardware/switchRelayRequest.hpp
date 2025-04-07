#pragma once

#include "httpServer/requestValidators/Request.hpp"
class SwitchRelayRequest : public Request
{
public:
    SwitchRelayRequest(PsychicRequest *request, JsonVariant &json) : Request(request, json)
    {
        rules = {
            {"header", "apiHeader"},
            {"relay", "required|integer|inArray:1&2"}};
    }
};