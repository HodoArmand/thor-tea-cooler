#pragma once

#include "requestValidator.hpp"
#include "requestBodyParameter.hpp"

struct StringKeyChecklistItem
{
    String key;
    bool check;
};

class Request
{

public:
    Request(AsyncWebServerRequest *request);
    ~Request();

    RequestValidator validator;
    //  TODO: Refractor idea: simplify how the Request->validate() functions work. validator should add the error messages more automatically.

    vector<RequestHeader> headers;
    vector<RequestBodyParameter> bodyParams;
    String method;
    int contentLength;
    String url;

    vector<String> requiredFields;

    void processRequestHeader(AsyncWebServerRequest *request);
    void processRequestBody(AsyncWebServerRequest *request);
    bool validate();
    vector<String> validationErrors;
    void addValidationError(String error);
    DynamicJsonDocument bodyToJson();

    bool hasBodyParam(String paramName);
    String getBodyParamValueByName(String paramName);

    bool hasHeader(String headerName);
    String getHeaderValueByName(String headerName);
    String getAuthApiKey();

    vector<String> getRequiredFields() const { return requiredFields; }
    void setRequiredFields(const vector<String> &requiredFields_)
    {
        requiredFields.resize(requiredFields_.size());
        requiredFields = requiredFields_;
    }
    bool checkRequiredFields();
};

Request::Request(AsyncWebServerRequest *request)
{
    processRequestHeader(request);
    processRequestBody(request);
    method = request->methodToString();
    contentLength = request->contentLength();
    url = request->url();
    validationErrors.clear();
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

inline DynamicJsonDocument Request::bodyToJson()
{
    DynamicJsonDocument json(1.5 * 255 * bodyParams.size());

    for (RequestBodyParameter param : bodyParams)
    {
        json[param.key] = param.value;
    }

    return json;
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

bool Request::hasHeader(String headerName)
{
    for (RequestHeader header : headers)
    {
        if (header.key == headerName)
        {
            return true;
        }
    }

    return false;
}
String Request::getHeaderValueByName(String headerName)
{
    for (RequestHeader header : headers)
    {
        if (header.key == headerName)
        {
            return header.value;
        }
    }

    return "";
}
String Request::getAuthApiKey()
{
    if (hasHeader("Authorization"))
    {
        String apiKey = getHeaderValueByName("Authorization");
        return apiKey.substring(7, apiKey.length());
    }
    else
    {
        return "";
    }
}

inline bool Request::checkRequiredFields()
{
    StringKeyChecklistItem fieldChecklist[requiredFields.size()];

    int i = 0;
    for (String requiredField : requiredFields)
    {
        fieldChecklist[i].key = requiredField;
        fieldChecklist[i].check = false;
        for (RequestBodyParameter bodyParam : bodyParams)
        {
            if (requiredField == bodyParam.key)
            {
                fieldChecklist[i].check = true;
                break;
            }
        }
        i++;
    }

    bool allFieldsPresent = true;

    for (StringKeyChecklistItem field : fieldChecklist)
    {
        if (!field.check)
        {
            addValidationError("Missing '" + field.key + "' request parameter.");

            if (allFieldsPresent)
            {
                allFieldsPresent = false;
            }
        }
    }

    return allFieldsPresent;
}
