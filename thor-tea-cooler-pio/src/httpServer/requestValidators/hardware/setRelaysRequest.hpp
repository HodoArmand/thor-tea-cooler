#pragma once

#include "httpServer/requestValidators/Request.hpp"

class SetRelaysRequest : public Request
{
public:
    SetRelaysRequest(PsychicRequest *request, JsonVariant &json) : Request(request, json)
    {
        rules = {
            {"header", "apiHeader"},
            {"relay1", "required|bool"},
            {"relay2", "required|bool"}};
    }
};