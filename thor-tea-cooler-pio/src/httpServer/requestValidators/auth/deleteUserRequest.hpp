#include "httpServer/requestValidators/Request.hpp"

class DeleteUserRequest : public Request
{

    using Request::Request;

public:
    bool validate();
};

//  delete itself
bool DeleteUserRequest::validate()
{
    if (!validator.isApiHeaderValid(headers))
    {
        addValidationError("Bad API header.");
    }

    setRequiredFields({"password", "password_confirmed"});

    if (!checkRequiredFields())
    {
        return false;
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
