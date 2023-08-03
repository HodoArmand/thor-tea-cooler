#pragma once

#include "httpServer/requestValidators/Request.hpp"

class SetServerConfigRequest : public Request
{

    using Request::Request;

public:
    bool validate();
};

bool SetServerConfigRequest::validate()
{
    if (!validator.isApiHeaderValid(headers))
    {
        addValidationError("Bad API header.");
    }

    setRequiredFields({"debugMode", "port", "apiKeyLength", "apiThrottleIntervalMs", "maxStoredUsers", "maxApiKeysPerUser", "maxApiKeysTotal", "selfHostMode"});

    if (!checkRequiredFields())
    {
        return false;
    }
    else
    {
        String debugMode = getBodyParamValueByName("debugMode");
        if (!validator.isBool(debugMode))
        {
            addValidationError("debugMode must be a boolean value.");
        }

        String port = getBodyParamValueByName("port");
        if (!validator.isStringInteger(port))
        {
            addValidationError("port must be an integer number.");
        }
        else if (!validator.between(port, 0, 65535))
        {
            addValidationError(port + " is not a valid port.");
        }

        String apiKeyLength = getBodyParamValueByName("apiKeyLength");
        if (!validator.isStringInteger(apiKeyLength))
        {
            addValidationError("apiKeyLength must be an integer number.");
        }
        else if (!validator.between(apiKeyLength, 10, 254))
        {
            addValidationError("apiKeyLength must be between 10 and 254");
        }

        String apiThrottleIntervalMs = getBodyParamValueByName("apiThrottleIntervalMs");
        if (!validator.isStringInteger(apiThrottleIntervalMs))
        {
            addValidationError("apiThrottleIntervalMs must be an integer number.");
        }
        else if (!validator.min(apiThrottleIntervalMs, 0))
        {
            addValidationError("apiThrottleIntervalMs must be at least 0.");
        }

        String maxStoredUsers = getBodyParamValueByName("maxStoredUsers");
        if (!validator.isStringInteger(maxStoredUsers))
        {
            addValidationError("maxStoredUsers must be an integer number.");
        }
        else if (!validator.min(maxStoredUsers, 1))
        {
            addValidationError("maxStoredUsers must be at least 1.");
        }

        String maxApiKeysPerUser = getBodyParamValueByName("maxApiKeysPerUser");
        if (!validator.isStringInteger(maxApiKeysPerUser))
        {
            addValidationError("maxApiKeysPerUser must be an integer number.");
        }
        else if (!validator.min(maxApiKeysPerUser, 1))
        {
            addValidationError("maxApiKeysPerUser must be at least 1.");
        }

        String maxApiKeysTotal = getBodyParamValueByName("maxApiKeysTotal");
        if (!validator.isStringInteger(maxApiKeysTotal))
        {
            addValidationError("maxApiKeysTotal must be an integer number.");
        }
        else if (!validator.min(maxApiKeysTotal, 1))
        {
            addValidationError("maxApiKeysTotal must be at least 1.");
        }

        String selfHostMode = getBodyParamValueByName("selfHostMode");
        if (!validator.isBool(selfHostMode))
        {
            addValidationError("selfHostMode must be a boolean value.");
        }

        if (validationErrors.size() != 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}
