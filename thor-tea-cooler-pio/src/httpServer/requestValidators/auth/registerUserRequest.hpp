#pragma once

#include "httpServer/requestValidators/Request.hpp"

class RegisterUserRequest : public Request
{

    using Request::Request;

public:
    bool validate();
};

bool RegisterUserRequest::validate()
{
    if (!validator.isApiHeaderValid(headers))
    {
        addValidationError("Bad API header.");
    }

    setRequiredFields({"name", "password", "password_confirmed"});

    if (!checkRequiredFields())
    {
        return false;
    }

    // TODO: add betweenLength validator prop
    String name = getBodyParamValueByName("name");
    if (!(validator.minLength(name, 3) && validator.maxLength(name, 32)))
    {
        addValidationError("Name must be between 3 and 32 characters long.");
    }

    String password = getBodyParamValueByName("password");
    if (!(validator.minLength(password, 8) && validator.maxLength(password, 32)))
    {
        addValidationError("Password must be between 8 and 32 characters long.");
    }

    String passwordConfirmed = getBodyParamValueByName("password_confirmed");
    if (password != passwordConfirmed)
    {
        addValidationError("Password confirmation must match the password field.");
    }

    return validationErrors.size() == 0;
}
