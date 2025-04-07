#pragma once

#include <Arduino.h>
#include <PsychicHttp.h>
#include <ArduinoJson.h>

class Controller
{
public:
    Controller(/* args */);
    ~Controller();

    static esp_err_t simpleResponse(PsychicRequest *request, int responseCode, String statusText, String msgText);
    static esp_err_t simpleOkResponse(PsychicRequest *request);
    static esp_err_t simpleCreatedResponse(PsychicRequest *request);
    static esp_err_t simpleUnauthorizedResponse(PsychicRequest *request);
    static esp_err_t simpleNotFoundResponse(PsychicRequest *request);

    static esp_err_t validationErrorsResponse(PsychicRequest *request, vector<String> validationErrors);
};

Controller::Controller(/* args */)
{
}

Controller::~Controller()
{
}

esp_err_t Controller::simpleResponse(PsychicRequest *request, int responseCode, String statusText, String msgText)
{
    PsychicJsonResponse response = PsychicJsonResponse(request);
    response.setCode(responseCode);
    response.setContentType("application/json");
    JsonObject json = response.getRoot();
    json["status"] = statusText;
    json["msg"] = msgText;

    return response.send();
}

esp_err_t Controller::simpleOkResponse(PsychicRequest *request)
{
    return simpleResponse(request, 200, "ok", "ok");
}

esp_err_t Controller::simpleCreatedResponse(PsychicRequest *request)
{
    return simpleResponse(request, 201, "ok", "ok");
}

esp_err_t Controller::simpleUnauthorizedResponse(PsychicRequest *request)
{
    return simpleResponse(request, 403, "Unauthorized.", "Unauthorized to access this resource.");
}

inline esp_err_t Controller::simpleNotFoundResponse(PsychicRequest *request)
{
    return simpleResponse(request, 404, "Not found.", "The requested resource/route was not found.");
}

esp_err_t Controller::validationErrorsResponse(PsychicRequest *request, vector<String> validationErrors)
{
    PsychicJsonResponse response = PsychicJsonResponse(request);
    response.setCode(400);
    response.setContentType("application/json");
    JsonObject json = response.getRoot();
    json["status"] = "Bad request.";
    json["msg"] = "Missing or incorrect request fields.";
    JsonArray errors = json["fieldErrors"].to<JsonArray>();
    for (String validationError : validationErrors)
    {
        errors.add(validationError);
    }

    return response.send();
}