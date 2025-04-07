#pragma once

#include "requestValidator.hpp"
#include "requestBodyParameter.hpp"
#include "requestRule.hpp"

struct StringKeyChecklistItem
{
    String key;
    bool check;
};

class Request
{

private:
    bool requiredFieldsSet = false;
    void validateHeaders(String headerRulesString);
    void validateRequestField(String ruleName, String rules);
    vector<String> validationRules = {
        "apiHeader",
        "loginApiHeader",
        "required",
        "same",
        "bool",
        "numeric",
        "integer",
        "float",
        "address",
        "secureAddress",
        "ipv4Address",
        "ipv4AddressShort",
        "gpioPin",
        "minLength",
        "maxLength",
        "between",
        "inArray"};

public:
    Request(PsychicRequest *request);
    Request(PsychicRequest *request, JsonVariant &json);
    ~Request();

    RequestValidator validator;

    vector<RequestHeader> headers;
    vector<RequestBodyParameter> bodyParams;
    String method;
    int contentLength;
    String url;

    vector<RequestRule> rules;
    vector<String> requiredFields;

    void processRequestHeader(PsychicRequest *request);
    void processRequestBody(JsonVariant &json);

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

Request::Request(PsychicRequest *request)
{
    processRequestHeader(request);
    method = request->methodStr();
    contentLength = request->contentLength();
    url = request->url();
    validationErrors.clear();
}

Request::Request(PsychicRequest *request, JsonVariant &json)
{
    processRequestHeader(request);
    processRequestBody(json);
    method = request->methodStr();
    contentLength = request->contentLength();
    url = request->url();
    validationErrors.clear();
}

Request::~Request()
{
}

inline void Request::validateHeaders(String headerRulesString)
{
    StringSplitter headerRules(headerRulesString, '|', 255);

    int rulesCount = headerRules.getItemCount();

    if (rulesCount < 1)
    {
        addValidationError("Formatting error at header validation.");
    }
    else
    {
        for (int i = 0; i < rulesCount; i++)
        {
            String headerRule = headerRules.getItemAtIndex(i);

            if (headerRule == "apiHeader" && !validator.isApiHeaderValid(headers))
            {
                addValidationError("Bad API header.");
            }
            else if (headerRule == "loginApiHeader" && !validator.isLoginApiHeaderValid(headers))
            {
                addValidationError("Bad login API header.");
            }
        }
    }
}

void Request::validateRequestField(String ruleField, String rules)
{
    StringSplitter rulesSplit(rules, '|', 255);

    int rulesCount = rulesSplit.getItemCount();

    if (rulesCount < 1)
    {
        addValidationError("Syntax error: " + ruleField + " field rule key exists but no validation rules given.");
    }
    else
    {
        String valueToValidate = getBodyParamValueByName(ruleField);

        for (int i = 0; i < rulesCount; i++)
        {
            String ruleString = rulesSplit.getItemAtIndex(i);

            if (ruleString == "required")
            {
                continue;
            }

            StringSplitter ruleParts(ruleString, ':', 255);
            bool rulehasParameters = ruleParts.getItemCount() > 1 ? true : false;

            if (!rulehasParameters)
            {
                String ruleName = ruleString;

                if (!validator.inArray(ruleName, validationRules))
                {
                    addValidationError("Error: The " + ruleName + " validation rule doesn't exist, @" + ruleField + ".");
                }

                if (ruleName == "bool" && !validator.isBool(valueToValidate))
                {
                    addValidationError(ruleField + " must be a boolean value.");
                }
                else if (ruleName == "numeric" && !validator.isStringNumeric(valueToValidate))
                {
                    addValidationError(ruleField + " must be a numeric value.");
                }
                else if (ruleName == "integer" && !validator.isStringInteger(valueToValidate))
                {
                    addValidationError(ruleField + " must be an integer number value.");
                }
                else if (ruleName == "float" && !validator.isFloat(valueToValidate))
                {
                    addValidationError(ruleField + " must be a float number value.");
                }
                else if (ruleName == "address" && !validator.isAddress(valueToValidate))
                {
                    addValidationError(ruleField + " must be an IP/address  value.");
                }
                else if (ruleName == "secureAddress" && !validator.isSecureAddress(valueToValidate))
                {
                    addValidationError(ruleField + " must be a secure IP/address  value.");
                }
                else if (ruleName == "ipv4Address" && !validator.isIpv4Address(valueToValidate))
                {
                    addValidationError(ruleField + " must be an IPv4 address  value.");
                }
                else if (ruleName == "ipv4AddressShort" && !validator.isIpv4Address(valueToValidate))
                {
                    addValidationError(ruleField + " must be an IPv4 address  value.");
                }
                else if (ruleName == "gpioPin" && Relay::validateIoPin(valueToValidate.toInt()) != PIN_OK)
                {
                    addValidationError(ruleField + ": " + valueToValidate + " is not a valid output pin.");
                }
            }
            else
            {
                String ruleName = ruleParts.getItemAtIndex(0);

                StringSplitter ruleParameters(ruleParts.getItemAtIndex(1), '&', 255);

                //  single arg validator rules
                if (ruleParameters.getItemCount() == 1)
                {
                    String ruleParameter = ruleParts.getItemAtIndex(1);

                    int ruleValueInt = ruleParameter.toInt();
                    float ruleValueFloat = ruleParameter.toFloat();

                    if (ruleName == "same" && valueToValidate != getBodyParamValueByName(ruleParameter))
                    {
                        addValidationError(ruleField + " must match " + ruleParameter + ".");
                    }
                    else if (ruleName == "minLength" && !validator.minLength(valueToValidate, ruleValueInt))
                    {
                        addValidationError(ruleField + " length must be at least " + ruleParameter + ".");
                    }
                    else if (ruleName == "maxLength" && !validator.maxLength(valueToValidate, ruleValueInt))
                    {
                        addValidationError(ruleField + " legth must be maximum " + ruleParameter + ".");
                    }
                    else if (ruleName == "min")
                    {
                        if (validator.isStringInteger(valueToValidate))
                        {
                            if (!validator.min(valueToValidate, ruleValueInt))
                            {
                                addValidationError(ruleField + " must be at least " + ruleParameter + ".");
                            }
                        }
                        else if (validator.isFloat(valueToValidate))
                        {
                            if (!validator.min(valueToValidate, ruleValueFloat))
                            {
                                addValidationError(ruleField + " must be at least " + ruleParameter + ".");
                            }
                        }
                        else
                        {
                            addValidationError("Invalid validation rule formatting at: " + ruleField + " -  " + ruleName + ":" + ruleParameter);
                        }
                    }
                    else if (ruleName == "max")
                    {
                        if (validator.isStringInteger(valueToValidate))
                        {
                            if (!validator.max(valueToValidate, ruleValueInt))
                            {
                                addValidationError(ruleField + " must be maximum " + ruleParameter + ".");
                            }
                        }
                        else if (validator.isFloat(valueToValidate))
                        {
                            if (!validator.max(valueToValidate, ruleValueFloat))
                            {
                                addValidationError(ruleField + " must be maximum " + ruleParameter + ".");
                            }
                        }
                        else
                        {
                            addValidationError("Invalid validation rule formatting at: " + ruleField + " -  " + ruleName + ":" + ruleParameter);
                        }
                    }
                }
                else if (ruleParameters.getItemCount() == 2) //  two arg validator rules
                {
                    String ruleParameter1 = ruleParameters.getItemAtIndex(0);
                    String ruleParameter2 = ruleParameters.getItemAtIndex(1);

                    int ruleParameterInt1 = ruleParameter1.toInt();
                    int ruleParameterInt2 = ruleParameter2.toInt();
                    float ruleParameterFloat1 = ruleParameter1.toFloat();
                    float ruleParameterFloat2 = ruleParameter2.toFloat();

                    if (ruleName == "between")
                    {
                        if (validator.isFloat(ruleParameter1) && validator.isFloat(ruleParameter2))
                        {
                            if (!validator.min(valueToValidate, ruleParameterFloat1) || !validator.max(valueToValidate, ruleParameterFloat2))
                            {
                                addValidationError(ruleField + ": " + valueToValidate + " must be between " + (String)ruleParameterFloat1 + " and " + (String)ruleParameterFloat2);
                            }
                        }
                        else if (validator.isStringInteger(ruleParameter1) && validator.isStringInteger(ruleParameter2))
                        {
                            if (!validator.min(valueToValidate, ruleParameterInt1) || !validator.max(valueToValidate, ruleParameterInt2))
                            {
                                addValidationError(ruleField + ": " + valueToValidate + " must be between " + (String)ruleParameterInt1 + " and " + (String)ruleParameterInt2);
                            }
                        }
                    }
                }
                //  multi arg validator rule (inArray)
                else
                {
                    if (ruleName == "inArray")
                    {
                        std::vector<String> ruleParamValues;
                        for (size_t i = 0; i < ruleParameters.getItemCount(); i++)
                        {
                            ruleParamValues.reserve(ruleParamValues.size() + 1);
                            ruleParamValues.push_back(ruleParameters.getItemAtIndex(i));
                        }

                        if (!validator.inArray(valueToValidate, ruleParamValues))
                        {
                            addValidationError(ruleField + " must be one of: " + ruleParts.getItemAtIndex(1));
                        }
                    }
                }
            }
        }
    }
}

bool Request::validate()
{
    if (!requiredFieldsSet)
    {
        for (RequestRule rule : rules)
        {
            if (rule.rules.indexOf("required") != -1)
            {
                requiredFields.reserve(requiredFields.size() + 1);
                requiredFields.push_back(rule.name);
            }
        }

        requiredFieldsSet = true;
    }

    if (!checkRequiredFields())
    {
        return false;
    }

    for (RequestRule rule : rules)
    {
        if (rule.name == "header")
        {
            validateHeaders(rule.rules);
        }
        else
        {
            validateRequestField(rule.name, rule.rules);
        }
    }

    return validationErrors.size() == 0;
}

inline void Request::processRequestHeader(PsychicRequest *request)
{
    String header_keys[3] = {"Content-Type", "Accept", "Authorization"};
    headers.reserve(3);

    RequestHeader headerValue;
    for (size_t i = 0; i < 3; i++)
    {
        String header_value = request->header(header_keys[i].c_str());
        headerValue.key = String(header_keys[i]);
        headerValue.value = String(header_value);
        headers.push_back(headerValue);
    }
}

inline void Request::processRequestBody(JsonVariant &json)
{
    JsonObject input = json.as<JsonObject>();

    for (JsonPair key_value_pair : input)
    {
        RequestBodyParameter bodyParam;
        bodyParam.key = String(key_value_pair.key().c_str());
        bodyParam.value = key_value_pair.value().as<String>();
        bodyParams.push_back(bodyParam);
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
