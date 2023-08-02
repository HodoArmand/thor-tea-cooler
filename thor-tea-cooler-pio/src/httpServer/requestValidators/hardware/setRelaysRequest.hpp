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
    if (!hasBodyParam("relay1"))
    {
        addValidationError("Missing 'relay1' request parameter.");
    }
    if (!hasBodyParam("relay2"))
    {
        addValidationError("Missing 'relay2' request parameter.");
    }
    if (hasBodyParam("relay1") && hasBodyParam("relay2"))
    {
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
