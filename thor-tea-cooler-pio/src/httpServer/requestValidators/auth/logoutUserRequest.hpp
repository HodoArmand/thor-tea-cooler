#pragma once

#include "httpServer/requestValidators/Request.hpp"

class LogoutUserRequest : public Request
{
public:
    LogoutUserRequest(AsyncWebServerRequest *request) : Request(request)
    {
        rules = {
            {"header", "apiHeader"}};
    }
};