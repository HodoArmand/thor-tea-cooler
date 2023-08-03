#pragma once

#include "httpServer/requestValidators/Request.hpp"

class SetNetworkConfigRequest : public Request
{

    using Request::Request;

public:
    bool validate();
};

bool SetNetworkConfigRequest::validate()
{
    if (!validator.isApiHeaderValid(headers))
    {
        addValidationError("Bad API header.");
    }

    setRequiredFields({"debugMode", "ssid", "password"});

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
