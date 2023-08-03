#pragma once

#include "httpServer/requestValidators/Request.hpp"

class SetHardwareConfigRequest : public Request
{

    using Request::Request;

public:
    bool validate();
};

bool SetHardwareConfigRequest::validate()
{
    if (!validator.isApiHeaderValid(headers))
    {
        addValidationError("Bad API header.");
    }

    //  TODO: refractor: use this new syntax at older, other required fields checks

    setRequiredFields({"debugMode", "relayIoPin1", "relayIoPin2", "oneWireIoPin", "temperatureSensorOffsetCelsius", "temperatureTargetDefault"});

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

        String relayIoPin1 = getBodyParamValueByName("relayIoPin1");
        if (!validator.isStringInteger(relayIoPin1))
        {
            addValidationError("relayIoPin1 must be an integer number.");
        }
        else if (Relay::validateIoPin(relayIoPin1.toInt()) != PIN_OK)
        {
            addValidationError("relayIoPin1 is not a valid output pin.");
        }

        String relayIoPin2 = getBodyParamValueByName("relayIoPin2");
        if (!validator.isStringInteger(relayIoPin2))
        {
            addValidationError("relayIoPin2 must be an integer number.");
        }
        else if (Relay::validateIoPin(relayIoPin2.toInt()) != PIN_OK)
        {
            addValidationError("relayIoPin2 is not a valid output pin.");
        }

        String oneWireIoPin = getBodyParamValueByName("oneWireIoPin");
        if (!validator.isStringInteger(oneWireIoPin))
        {
            addValidationError("oneWireIoPin must be an integer number.");
        }

        String temperatureSensorOffsetCelsius = getBodyParamValueByName("temperatureSensorOffsetCelsius");
        if (!validator.isFloat(temperatureSensorOffsetCelsius))
        {
            addValidationError("temperatureSensorOffsetCelsius must be a float number.");
        }

        String temperatureTargetDefault = getBodyParamValueByName("temperatureTargetDefault");
        if (!validator.isFloat(temperatureTargetDefault))
        {
            addValidationError("temperatureTargetDefault must be a float number.");
        }
        else if (!validator.between(temperatureTargetDefault, 20.00f, 75.00f))
        {
            addValidationError("temperatureTargetDefault must be between 20 and 75 celsius.");
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
