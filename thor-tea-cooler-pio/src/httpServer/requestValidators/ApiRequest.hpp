#pragma once

#include "httpServer/requestValidators/Request.hpp"

class ApiRequest : public Request
{
    using Request::Request;

public:
    bool validate();
};

bool ApiRequest::validate()
{
    if (!validator.isApiHeaderValid(headers))
    {
        addValidationError("Bad API header.");
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
