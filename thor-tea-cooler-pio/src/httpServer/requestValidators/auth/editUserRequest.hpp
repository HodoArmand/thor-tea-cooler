#include "httpServer/requestValidators/Request.hpp"

class EditUserRequest : public Request
{

    using Request::Request;

public:
    bool validate();
};

bool EditUserRequest::validate()
{
    if (!validator.isApiHeaderValid(headers))
    {
        addValidationError("Bad API header.");
    }

    if (!hasBodyParam("name") && !hasBodyParam("password"))
    {
        addValidationError("Missing 'name' and 'password' request parameter, nothing to edit.");
    }
    else
    {
        if (hasBodyParam("name"))
        {
            String name = getBodyParamValueByName("name");
            if (!(validator.minLength(name, 3) && validator.maxLength(name, 32)))
            {
                addValidationError("Name must be between 3 and 32 characters long.");
            }
        }
        if (hasBodyParam("password"))
        {
            String password = getBodyParamValueByName("password");
            if (!(validator.minLength(password, 8) && validator.maxLength(password, 32)))
            {
                addValidationError("Pasword must be between 8 and 32 characters long.");
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
