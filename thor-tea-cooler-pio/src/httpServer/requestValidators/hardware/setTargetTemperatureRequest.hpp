#pragma once

#include "httpServer/requestValidators/Request.hpp"

class SetTargetTemperatureRequest : public Request
{
    using Request::Request;

public:
    bool validate();
};

bool SetTargetTemperatureRequest::validate()
{
    if (!validator.isApiHeaderValid(headers))
    {
        addValidationError("Bad API header.");
    }
    if (!hasBodyParam("targetTemperature"))
    {
        addValidationError("Missing 'targetTemperature' request parameter.");
    }
    else
    {
        String targetTemperature = getBodyParamValueByName("targetTemperature");
        if (!validator.isFloat(targetTemperature) && !validator.isStringInteger(targetTemperature))
        {
            addValidationError("targetTemperature must be a number.");
        }
        else
        {
            //  TODO: add minmax config for target temperature or just keep it const validation?
            if (!validator.between(targetTemperature, 20.00f, 75.00f))
            {
                addValidationError("targetTemperature must be between 20 and 75 celsius.");
            }
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
