#pragma once

#include "httpServer/requestValidators/Request.hpp"

class SetServerConfigRequest : public Request
{
public:
    SetServerConfigRequest(AsyncWebServerRequest *request) : Request(request)
    {
        rules = {
            {"header", "apiHeader"},
            {"debugMode", "required|bool"},
            {"port", "required|integer|between:0&65535"},
            {"apiKeyLength", "required|integer|between:10&254"},
            {"apiThrottleIntervalMs", "required|integer|min:0"},
            {"maxStoredUsers", "required|integer|min:1"},
            {"maxApiKeysPerUser", "required|integer|min:1"},
            {"maxApiKeysTotal", "required|integer|min:1"},
            {"selfHostMode", "required|bool"}};
    }
};
