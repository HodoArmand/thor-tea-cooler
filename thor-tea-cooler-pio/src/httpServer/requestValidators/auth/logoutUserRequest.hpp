#include "httpServer/requestValidators/Request.hpp"

class LogoutUserRequest : public Request
{
    
    using Request::Request;

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
