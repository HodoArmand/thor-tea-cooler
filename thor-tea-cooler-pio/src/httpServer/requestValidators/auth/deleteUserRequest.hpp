#include "httpServer/requestValidators/Request.hpp"

class DeleteUserRequest : Request
{
public:
    bool validate();
};

bool DeleteUserRequest::validate()
{
    if (!validator.isApiHeaderValid(headers))
    {
        addValidationError("Bad API header.");
    }
    if (!hasBodyParam("userId"))
    {
        addValidationError("Missing 'userId' request parameter.");
    }
    else
    {
        String userId = getBodyParamValueByName("userId");
        if (!(validator.isStringInteger(userId) && userId.toInt() >= 0))
        {
            addValidationError("User id must be a positive integer.");
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
