#include "httpServer/requestValidators/Request.hpp"

class LoginUserRequest : public Request
{
    using Request::Request;

public:
    bool validate();
};

bool LoginUserRequest::validate()
{
    if (!validator.isLoginApiHeaderValid(headers))
    {
        addValidationError("Bad API header.");
    }

    setRequiredFields({"name", "password"});

    if (!checkRequiredFields())
    {
        return false;
    }

    String name = getBodyParamValueByName("name");
    if (!(validator.minLength(name, 3) && validator.maxLength(name, 32)))
    {
        addValidationError("Name must be between 3 and 32 characters long.");
    }

    String password = getBodyParamValueByName("password");
    if (!(validator.minLength(password, 8) && validator.maxLength(password, 32)))
    {
        addValidationError("Pasword must be between 8 and 32 characters long.");
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
