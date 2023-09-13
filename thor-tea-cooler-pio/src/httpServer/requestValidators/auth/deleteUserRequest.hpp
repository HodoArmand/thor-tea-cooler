#pragma once

#include "httpServer/requestValidators/Request.hpp"

class DeleteUserRequest : public Request
{

public:
    DeleteUserRequest(AsyncWebServerRequest *request) : Request(request)
    {
        rules = {
            {"header", "apiHeader"},
            {"password", "required|minLength:8|maxLength:32"},
            {"password_confirmed", "required|same:password"}};
    }
};
