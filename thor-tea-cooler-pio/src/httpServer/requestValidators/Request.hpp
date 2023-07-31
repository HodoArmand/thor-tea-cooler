
#include "requestValidator.hpp"
#include "requestBodyParameter.hpp"

class Request
{

public:
    Request(AsyncWebServerRequest *request);
    ~Request();

    RequestValidator validator;

    vector<RequestHeader> headers;
    vector<RequestBodyParameter> bodyParams;
    String method;
    int contentLength;
    String url;

    void processRequestHeader(AsyncWebServerRequest *request);
    void processRequestBody(AsyncWebServerRequest *request);
    bool validate();
    vector<String> validationErrors;
    void addValidationError(String error);

    bool hasBodyParam(String paramName);
    String getBodyParamValueByName(String paramName);
};

Request::Request(AsyncWebServerRequest *request)
{
    processRequestHeader(request);
    processRequestBody(request);
    method = request->methodToString();
    contentLength = request->contentLength();
    url = request->url();
    validationErrors.clear();
    validationErrors.resize(1);
}

Request::~Request()
{
}

inline bool Request::validate()
{
    return true;
}

inline void Request::processRequestHeader(AsyncWebServerRequest *request)
{
    int headerCount = request->headers();
    if (headerCount != 0)
    {
        headers.reserve(headerCount);
        RequestHeader headerValue;
        for (size_t i = 0; i < headerCount; i++)
        {
            AsyncWebHeader *header = request->getHeader(i);
            headerValue.key = String(request->headerName(i));
            headerValue.value = String(request->header(i));
            headers.push_back(headerValue);
        }
    }
}

inline void Request::processRequestBody(AsyncWebServerRequest *request)
{
    int bodyParameterCount = request->params();
    if (bodyParameterCount != 0)
    {
        bodyParams.reserve(bodyParameterCount);
        RequestBodyParameter bodyParam;
        for (size_t i = 0; i < bodyParameterCount; i++)
        {
            AsyncWebParameter *parameter = request->getParam(i);
            bodyParam.key = String(parameter->name());
            bodyParam.value = String(parameter->value());
            bodyParams.push_back(bodyParam);
        }
    }
}

bool Request::hasBodyParam(String paramName)
{
    for (RequestBodyParameter param : bodyParams)
    {
        if (param.key == paramName)
        {
            return true;
        }
    }

    return false;
}

void Request::addValidationError(String error)
{
    validationErrors.reserve(validationErrors.size() + 1);
    validationErrors.push_back(error);
}

String Request::getBodyParamValueByName(String paramName)
{
    for (RequestBodyParameter param : bodyParams)
    {
        if (param.key == paramName)
        {
            return param.value;
        }
    }

    return "";
}