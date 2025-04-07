#pragma once

#include "httpServer/requestValidators/Request.hpp"

class EditUserRequest : public Request
{
public:
    EditUserRequest(PsychicRequest *request, JsonVariant &json) : Request(request, json)
    {
        rules = {
            {"header", "apiHeader"},
            {"name", "required|minLength:3|maxLength:32"},
            {"password", "required|minLength:8|maxLength:32"},
            {"password_confirmed", "required|same:password"}};
    }
};
