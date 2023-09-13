#pragma once

#include "httpServer/requestValidators/Request.hpp"

class SetRelaysRequest : public Request
{
public:
    SetRelaysRequest(AsyncWebServerRequest *request) : Request(request)
    {
        rules = {
            {"header", "apiHeader"},
            {"relay1", "required|bool"},
            {"relay2", "required|bool"}};
    }
};