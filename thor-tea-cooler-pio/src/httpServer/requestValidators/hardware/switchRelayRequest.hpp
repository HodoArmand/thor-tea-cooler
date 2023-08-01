#include "httpServer/requestValidators/Request.hpp"

class SwitchRelayRequest : public Request
{
    using Request::Request;

public:
    bool validate();
};

bool SwitchRelayRequest::validate()
{
    if (!validator.isApiHeaderValid(headers))
    {
        addValidationError("Bad API header.");
    }
    if (!hasBodyParam("relay"))
    {
        addValidationError("Missing 'relay' request parameter.");
    }
    else
    {
        String relay = getBodyParamValueByName("relay");
        if (!(relay == "1" || relay == "2"))
        {
            addValidationError("Bad relay number.");
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
