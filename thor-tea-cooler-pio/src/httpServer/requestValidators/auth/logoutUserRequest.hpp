#include "httpServer/requestValidators/Request.hpp"

class LogoutUserRequest : Request
{
public:
    bool validate();
};

bool LogoutUserRequest::validate()
{
    if (!validator.isApiHeaderValid(headers))
    {
        addValidationError("Bad API header.");
        return false;
    }
    else
    {
        return true;
    }
}
