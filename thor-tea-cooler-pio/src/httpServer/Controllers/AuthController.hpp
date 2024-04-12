#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include "httpServer/Controllers/Controller.hpp"
#include "httpServer/Authorization.hpp"
#include "httpServer/requestValidators/auth/loginUserRequest.hpp"
#include "httpServer/requestValidators/auth/logoutUserRequest.hpp"
#include "httpServer/requestValidators/auth/registerUserRequest.hpp"
#include "httpServer/requestValidators/auth/editUserRequest.hpp"
#include "httpServer/requestValidators/auth/deleteUserRequest.hpp"

class AuthController : Controller
{
private:
    Authorization *auth;

public:
    AuthController(Authorization *auth_);
    ~AuthController();

    void login(AsyncWebServerRequest *request_);
    void logout(AsyncWebServerRequest *request_);
    void registerUser(AsyncWebServerRequest *request_);
    void editUser(AsyncWebServerRequest *request_);
    void deleteUser(AsyncWebServerRequest *request_);
};

AuthController::AuthController(Authorization *auth_)
{
    auth = auth_;
}

AuthController::~AuthController()
{
}

void AuthController::login(AsyncWebServerRequest *request_)
{

    LoginUserRequest request(request_);

    if (!request.validate())
    {
        validationErrorsResponse(request_, request.validationErrors);
    }
    else
    {
        String authResult = auth->loginUser(request.getBodyParamValueByName("name"), request.getBodyParamValueByName("password"));

        if (authResult == "0")
        {
            simpleResponse(request_, 401, "Bad credentials.", "The provided login credentials don't match any of our records.");
        }
        else if (authResult == "ERROR: DISK IO")
        {
            simpleResponse(request_, 500, "Serverside IO error.", "The provided login credentials are correct, but there has been an error when saving them to our system.");
        }
        else
        {
            simpleResponse(request_, 201, "ok", authResult);
        }
    }
}

void AuthController::logout(AsyncWebServerRequest *request_)
{
    LogoutUserRequest request(request_);

    if (!request.validate())
    {
        validationErrorsResponse(request_, request.validationErrors);
    }
    else
    {
        int userId = auth->findUserByApiKey(request.getAuthApiKey()).getId();

        if (userId == -1)
        {
            simpleUnauthorizedResponse(request_);
        }
        else
        {
            if (!auth->logoutUser(userId))
            {
                simpleResponse(request_, 500, "Serverside IO error.", "The provided logout credentials are correct, but there has been an error when saving the changes to our system.");
            }
            else
            {
                simpleCreatedResponse(request_);
            }
        }
    }
}

void AuthController::registerUser(AsyncWebServerRequest *request_)
{
    RegisterUserRequest request(request_);

    if (!request.validate())
    {
        validationErrorsResponse(request_, request.validationErrors);
    }
    else
    {
        int userId = auth->findUserByApiKey(request.getAuthApiKey()).getId();

        if (userId == -1)
        {
            simpleUnauthorizedResponse(request_);
        }
        else
        {

            RegisterUserResult registerUserResult = auth->registerUser(request.getBodyParamValueByName("name"), request.getBodyParamValueByName("password"));

            if (!registerUserResult == REG_USR_OK)
            {
                if (registerUserResult == REG_USR_NAME_TAKEN)
                {
                    simpleResponse(request_, 400, "Name taken.", "The provided name is already taken.");
                }
                else if (registerUserResult == REG_USR_DISK_ERR)
                {
                    simpleResponse(request_, 500, "Serverside IO error.", "The provided credentials are correct, but there has been an error when saving them to our system.");
                }
            }
            else
            {
                simpleCreatedResponse(request_);
            }
        }
    }
}
void AuthController::editUser(AsyncWebServerRequest *request_)
{
    EditUserRequest request(request_);

    if (!request.validate())
    {
        validationErrorsResponse(request_, request.validationErrors);
    }
    else
    {
        User user = auth->findUserByApiKey(request.getAuthApiKey());

        if (user.getId() == -1)
        {
            simpleUnauthorizedResponse(request_);
        }
        else
        {
            if (request.hasBodyParam("name"))
            {
                user.setName(request.getBodyParamValueByName("name"));
            }
            if (request.hasBodyParam("password"))
            {
                user.setPassword(request.getBodyParamValueByName("password"));
            }

            if (!auth->editUser(user.getId(), user.getName(), user.getPassword()))
            {
                simpleResponse(request_, 500, "Serverside IO error.", "The provided credentials are correct, but there has been an error when saving them to our system.");
            }
            else
            {
                simpleCreatedResponse(request_);
            }
        }
    }
}
void AuthController::deleteUser(AsyncWebServerRequest *request_)
{
    DeleteUserRequest request(request_);

    if (!request.validate())
    {
        validationErrorsResponse(request_, request.validationErrors);
    }
    else
    {
        User user = auth->findUserByApiKey(request.getAuthApiKey());

        if (user.getId() == -1)
        {
            simpleUnauthorizedResponse(request_);
        }
        else
        {
            if (request.getBodyParamValueByName("password") != user.getPassword())
            {
                simpleResponse(request_, 401, "Bad credentials.", "Delete operation refused: Wrong password.");
            }
            else if (!auth->deleteUser(user.getId()))
            {
                simpleResponse(request_, 500, "Serverside IO error.", "The provided credentials are correct, but there has been an error when saving them to our system.");
            }
            else if (!auth->loadUsersFromDisk())
            {
                simpleResponse(request_, 500, "Serverside IO error.", "The user was deleted but there was an error when loading the new DB state.");
            }
            else
            {
                simpleCreatedResponse(request_);
            }
        }
    }
}
