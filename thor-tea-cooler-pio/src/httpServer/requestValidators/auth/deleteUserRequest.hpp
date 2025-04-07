#pragma once

#include "httpServer/requestValidators/Request.hpp"

class DeleteUserRequest : public Request
{

public:
    DeleteUserRequest(PsychicRequest *request, JsonVariant &json) : Request(request, json)
    {
        rules = {
            {"header", "apiHeader"},
            {"password", "required|minLength:8|maxLength:32"},
            {"password_confirmed", "required|same:password"}};
    }
};
