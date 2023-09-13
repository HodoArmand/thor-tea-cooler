#pragma once

#include "httpServer/requestValidators/Request.hpp"

class ApiRequest : public Request
{
public:
    ApiRequest(AsyncWebServerRequest *request) : Request(request)
    {
        rules = {
            {"header", "apiHeader"}};
    }
};