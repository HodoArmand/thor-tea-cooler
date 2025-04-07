#pragma once

#include "httpServer/requestValidators/Request.hpp"

class ApiRequest : public Request
{
public:
    ApiRequest(PsychicRequest *request) : Request(request)
    {
        rules = {
            {"header", "apiHeader"}};
    }
};