#pragma once

#include <Arduino.h>
#define PSY_ENABLE_SSL true
#define CONFIG_ESP_HTTPS_SERVER_ENABLE
#include <PsychicHttpsServer.h>

#include "httpServer/Controllers/Controller.hpp"
#include "httpServer/Authorization.hpp"
#include "httpServer/requestValidators/ApiRequest.hpp"
#include "httpServer/requestValidators/auth/loginUserRequest.hpp"
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

    esp_err_t login(PsychicRequest *request_, JsonVariant &json);
    esp_err_t logout(PsychicRequest *request_);
    esp_err_t registerUser(PsychicRequest *request_, JsonVariant &json);
    esp_err_t editUser(PsychicRequest *request_, JsonVariant &json);
    esp_err_t deleteUser(PsychicRequest *request_, JsonVariant &json);
};

AuthController::AuthController(Authorization *auth_)
{
    auth = auth_;
}

AuthController::~AuthController()
{
}

esp_err_t AuthController::login(PsychicRequest *request_, JsonVariant &json)
{

    LoginUserRequest request(request_, json);
    esp_err_t response;

    if (!request.validate())
    {
        response = validationErrorsResponse(request_, request.validationErrors);
    }
    else
    {
        String authResult = auth->loginUser(request.getBodyParamValueByName("name"), request.getBodyParamValueByName("password"));

        if (authResult == "0")
        {
            response = simpleResponse(request_, 401, "Bad credentials.", "The provided login credentials don't match any of our records.");
        }
        else if (authResult == "ERROR: DISK IO")
        {
            response = simpleResponse(request_, 500, "Serverside IO error.", "The provided login credentials are correct, but there has been an error when saving them to our system.");
        }
        else
        {
            response = simpleResponse(request_, 201, "ok", authResult);
        }
    }

    return response;
}

esp_err_t AuthController::logout(PsychicRequest *request_)
{
    ApiRequest request(request_);
    esp_err_t response;

    if (!request.validate())
    {
        response = validationErrorsResponse(request_, request.validationErrors);
    }
    else
    {
        int userId = auth->findUserByApiKey(request.getAuthApiKey()).getId();

        if (userId == -1)
        {
            response = simpleUnauthorizedResponse(request_);
        }
        else
        {
            if (!auth->logoutUser(userId))
            {
                response = simpleResponse(request_, 500, "Serverside IO error.", "The provided logout credentials are correct, but there has been an error when saving the changes to our system.");
            }
            else
            {
                response = simpleCreatedResponse(request_);
            }
        }
    }

    return response;
}

esp_err_t AuthController::registerUser(PsychicRequest *request_, JsonVariant &json)
{
    RegisterUserRequest request(request_, json);
    esp_err_t response;

    if (!request.validate())
    {
        response = validationErrorsResponse(request_, request.validationErrors);
    }
    else
    {
        int userId = auth->findUserByApiKey(request.getAuthApiKey()).getId();

        if (userId == -1)
        {
            response = simpleUnauthorizedResponse(request_);
        }
        else
        {

            RegisterUserResult registerUserResult = auth->registerUser(request.getBodyParamValueByName("name"), request.getBodyParamValueByName("password"));

            if (!registerUserResult == REG_USR_OK)
            {
                if (registerUserResult == REG_USR_NAME_TAKEN)
                {
                    response = simpleResponse(request_, 400, "Name taken.", "The provided name is already taken.");
                }
                else if (registerUserResult == REG_USR_DISK_ERR)
                {
                    response = simpleResponse(request_, 500, "Serverside IO error.", "The provided credentials are correct, but there has been an error when saving them to our system.");
                }
            }
            else
            {
                response = simpleCreatedResponse(request_);
            }
        }
    }

    return response;
}
esp_err_t AuthController::editUser(PsychicRequest *request_, JsonVariant &json)
{
    EditUserRequest request(request_, json);
    esp_err_t response;

    if (!request.validate())
    {
        return validationErrorsResponse(request_, request.validationErrors);
    }
    else
    {
        User user = auth->findUserByApiKey(request.getAuthApiKey());

        if (user.getId() == -1)
        {
            response = simpleUnauthorizedResponse(request_);
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
                response = simpleResponse(request_, 500, "Serverside IO error.", "The provided credentials are correct, but there has been an error when saving them to our system.");
            }
            else
            {
                response = simpleCreatedResponse(request_);
            }
        }
    }

    return response;
}
esp_err_t AuthController::deleteUser(PsychicRequest *request_, JsonVariant &json)
{
    DeleteUserRequest request(request_, json);
    esp_err_t response;

    if (!request.validate())
    {
        response = validationErrorsResponse(request_, request.validationErrors);
    }
    else
    {
        User user = auth->findUserByApiKey(request.getAuthApiKey());

        if (user.getId() == -1)
        {
            response = simpleUnauthorizedResponse(request_);
        }
        else
        {
            if (request.getBodyParamValueByName("password") != user.getPassword())
            {
                response = simpleResponse(request_, 401, "Bad credentials.", "Delete operation refused: Wrong password.");
            }
            else if (!auth->deleteUser(user.getId()))
            {
                response = simpleResponse(request_, 500, "Serverside IO error.", "The provided credentials are correct, but there has been an error when saving them to our system.");
            }
            else if (!auth->loadUsersFromDisk())
            {
                response = simpleResponse(request_, 500, "Serverside IO error.", "The user was deleted but there was an error when loading the new DB state.");
            }
            else
            {
                response = simpleCreatedResponse(request_);
            }
        }
    }

    return response;
}
