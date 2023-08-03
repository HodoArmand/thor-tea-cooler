#pragma once

#include "httpServer/requestValidators/Request.hpp"

class SetRelaysRequest : public Request
{
    using Request::Request;

public:
    bool validate();
};

bool SetRelaysRequest::validate()
{
    if (!validator.isApiHeaderValid(headers))
    {
        addValidationError("Bad API header.");
    }

    setRequiredFields({"relay1", "relay2"});

    if (!checkRequiredFields())
    {
        return false;
    }

    String relay = getBodyParamValueByName("relay1");
    if (!validator.isBool(relay))
    {
        addValidationError("Bad relay1 value. Must be true/false or 0/1.");
    }
    relay = getBodyParamValueByName("relay2");
    if (!validator.isBool(relay))
    {
        addValidationError("Bad relay2 value. Must be true/false or 0/1.");
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
