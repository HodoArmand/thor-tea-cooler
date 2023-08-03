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

    if (validationErrors.size() != 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}
