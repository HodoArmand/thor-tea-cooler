#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

class Controller
{
public:
    Controller(/* args */);
    ~Controller();

    void simpleResponse(AsyncWebServerRequest *request, int responseCode, String statusText, String msgText);
    void simpleBigResponse(AsyncWebServerRequest *request, int responseCode, String statusText, String msgText);
    void simpleVeryBigResponse(AsyncWebServerRequest *request, int responseCode, String statusText, String msgText);
    void simpleOkResponse(AsyncWebServerRequest *request);
    void simpleCreatedResponse(AsyncWebServerRequest *request);
    void simpleUnauthorizedResponse(AsyncWebServerRequest *request);

    void validationErrorsResponse(AsyncWebServerRequest *request, vector<String> validationErrors);
};

Controller::Controller(/* args */)
{
}

Controller::~Controller()
{
}

void Controller::simpleResponse(AsyncWebServerRequest *request, int responseCode, String statusText, String msgText)
{
    String responseBody;
    DynamicJsonDocument json(200);
    json["status"] = statusText;
    json["msg"] = msgText;
    serializeJsonPretty(json, responseBody);
    json.clear();

    request->send(responseCode, "application/json", responseBody);
}

void Controller::simpleBigResponse(AsyncWebServerRequest *request, int responseCode, String statusText, String msgText)
{
    String responseBody;
    DynamicJsonDocument json(2048);
    json["status"] = statusText;
    json["msg"] = msgText;
    serializeJsonPretty(json, responseBody);
    json.clear();

    request->send(responseCode, "application/json", responseBody);
}

void Controller::simpleVeryBigResponse(AsyncWebServerRequest *request, int responseCode, String statusText, String msgText)
{
    String responseBody;
    DynamicJsonDocument json(5000);
    json["status"] = statusText;
    json["msg"] = msgText;
    serializeJsonPretty(json, responseBody);
    json.clear();

    request->send(responseCode, "application/json", responseBody);
}

void Controller::simpleOkResponse(AsyncWebServerRequest *request)
{
    String responseBody;
    DynamicJsonDocument json(200);
    json["status"] = "ok";
    json["msg"] = "ok";
    serializeJsonPretty(json, responseBody);
    json.clear();
    request->send(200, "application/json", responseBody);
}

void Controller::simpleCreatedResponse(AsyncWebServerRequest *request)
{
    String responseBody;
    DynamicJsonDocument json(200);
    json["status"] = "ok";
    json["msg"] = "ok";
    serializeJsonPretty(json, responseBody);
    json.clear();

    request->send(201, "application/json", responseBody);
}

void Controller::simpleUnauthorizedResponse(AsyncWebServerRequest *request)
{

    String responseBody;
    DynamicJsonDocument json(200);
    json["status"] = "Unauthorized.";
    json["msg"] = "Unauthorized to access this resource.";
    serializeJsonPretty(json, responseBody);
    json.clear();

    request->send(403, "application/json", responseBody);
}

void Controller::validationErrorsResponse(AsyncWebServerRequest *request, vector<String> validationErrors)
{
    String responseBody;
    DynamicJsonDocument json(2048);
    json["status"] = "Bad request.";
    json["msg"] = "Missing or incorrect request fields.";

    JsonArray fieldErrorsJson = json.createNestedArray("fieldErrors");

    for (String validationError : validationErrors)
    {
        fieldErrorsJson.add(validationError);
    }

    serializeJsonPretty(json, responseBody);
    request->send(400, "application/json", responseBody);
}