#pragma once

#include "httpServer/requestValidators/Request.hpp"

class LoginUserRequest : public Request
{
public:
    LoginUserRequest(PsychicRequest *request, JsonVariant &json) : Request(request, json)
    {
        rules = {
            {"header", "loginApiHeader"},
            {"name", "required|minLength:3|maxLength:32"},
            {"password", "required|minLength:8|maxLength:32"}};
    }
};
