#pragma once

#include "httpServer/requestValidators/Request.hpp"

class RegisterUserRequest : public Request
{
public:
    RegisterUserRequest(AsyncWebServerRequest *request) : Request(request)
    {
        rules = {
            {"header", "apiHeader"},
            {"name", "required|minLength:3|maxLength:32"},
            {"password", "required|minLength:8|maxLength:32"},
            {"password_confirmed", "required|same:password"}};
    }
};