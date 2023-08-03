#pragma once

#include <Arduino.h>
#include <vector>
#include <algorithm>
#include <StringSplitter.h>
#include <ESPAsyncWebServer.h>

#include "httpServer/ApiKey.hpp"
#include "hardware/relay.hpp"

using namespace std;

struct RequestHeader
{
    String key;
    String value;
};

class RequestValidator
{
private:
    String removeMinusSign(String string_);

public:
    RequestValidator();
    ~RequestValidator();

    bool inArray(const String &value, const std::vector<String> &array);

    //  TODO: properly test ALL vlidator types

    bool stringtoBool(String string_);
    bool isBool(String string_);
    bool isStringNumeric(String string_);
    bool isStringInteger(String string_);
    bool isFloat(String string_);

    bool minLength(String string_, int minLength);
    bool maxLength(String string_, int maxLength);
    bool min(String string_, int minValue);
    bool min(String string_, float minValue);
    bool max(String string_, int maxValue);
    bool max(String string_, float maxValue);
    bool between(String string_, int minValue, int maxValue);
    bool between(String string_, float minValue, float maxValue);

    bool isAddress(String string_);
    bool isSecureAddress(String string_);
    bool isIpv4Address(String string_);
    bool isIpv4AddressShort(String string_);

    bool isLoginApiHeaderValid(vector<RequestHeader> headers);
    bool isApiHeaderValid(vector<RequestHeader> headers);
};

RequestValidator::RequestValidator()
{
}

RequestValidator::~RequestValidator()
{
}

inline String RequestValidator::removeMinusSign(String string_)
{
    if (string_[0] == '-')
    {
        string_ = string_.substring(1, string_.length() - 1);
    }

    return string_;
}

inline bool RequestValidator::inArray(const String &value, const std::vector<String> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}

inline bool RequestValidator::stringtoBool(String string_)
{
    if (string_ == "true" || string_ == "1")
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline bool RequestValidator::isBool(String string_)
{
    std::vector<String> valuesParseableAsBool{
        "0",
        "1",
        "true",
        "false",
    };
    return inArray(string_, valuesParseableAsBool);
}

inline bool RequestValidator::isStringNumeric(String string_)
{
    string_ = removeMinusSign(string_);
    for (char inputChar : string_)
    {
        if (!isDigit(inputChar) && inputChar != '.' && inputChar != ',')
        {
            return false;
        }
    }
    return true;
}

inline bool RequestValidator::isStringInteger(String string_)
{
    string_ = removeMinusSign(string_);
    for (char inputChar : string_)
    {
        if (!isDigit(inputChar))
        {
            return false;
        }
    }

    return true;
}

inline bool RequestValidator::isFloat(String string_)
{
    string_ = removeMinusSign(string_);
    bool hasOneDecimator = false;
    int numOfDecimators = 0;
    for (char letter : string_)
    {
        if (letter == ',' || letter == '.')
        {
            numOfDecimators++;
        }
    }
    if (numOfDecimators == 1)
    {
        hasOneDecimator = true;
    }

    return isStringNumeric(string_) && hasOneDecimator;
}

inline bool RequestValidator::minLength(String string_, int minLength)
{
    return string_.length() >= minLength;
}

inline bool RequestValidator::maxLength(String string_, int maxLength)
{
    return string_.length() <= maxLength;
}

inline bool RequestValidator::min(String string_, int minValue)
{
    return string_.toInt() >= minValue;
}

inline bool RequestValidator::max(String string_, int maxValue)
{
    return string_.toInt() >= maxValue;
}

inline bool RequestValidator::min(String string_, float minValue)
{
    return string_.toFloat() >= minValue;
}

inline bool RequestValidator::max(String string_, float maxValue)
{
    return string_.toFloat() >= maxValue;
}

inline bool RequestValidator::between(String string_, int minValue, int maxValue)
{
    if (!isStringInteger(string_))
    {
        return false;
    }
    else
    {
        int value = string_.toInt();
        return value >= minValue && value <= maxValue;
    }
}

inline bool RequestValidator::between(String string_, float minValue, float maxValue)
{
    if (!isStringNumeric(string_))
    {
        return false;
    }
    else
    {
        float value = string_.toFloat();
        return value >= minValue && value <= maxValue;
    }
}

inline bool RequestValidator::isAddress(String string_)
{
    if (string_.length() > 7 && string_.substring(0, 6) == R"(http://)")
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline bool RequestValidator::isSecureAddress(String string_)
{
    if (string_.length() > 8 && string_.substring(0, 7) == R"(https://)")
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline bool RequestValidator::isIpv4Address(String string_)
{
    if (isAddress(string_))
    {
        string_ = string_.substring(7, string_.length() - 1);
    }
    else if (isSecureAddress(string_))
    {
        string_ = string_.substring(8, string_.length() - 1);
    }
    else
    {
        return false;
    }

    StringSplitter addressTokens(string_, '.', 5);

    if (addressTokens.getItemCount() != 4)
    {
        return false;
    }

    String addressToken;
    int tokenValue;
    for (size_t i = 0; i < 4; i++)
    {
        addressToken = addressTokens.getItemAtIndex(i);
        if (!isStringInteger(addressToken))
        {
            return false;
        }
        else
        {
            tokenValue = addressToken.toInt();
            if (tokenValue < 0 || tokenValue > 255)
            {
                return false;
            }
        }
    }

    return true;
}

inline bool RequestValidator::isIpv4AddressShort(String string_)
{
    StringSplitter addressTokens(string_, '.', 5);

    if (addressTokens.getItemCount() != 4)
    {
        return false;
    }

    String addressToken;
    int tokenValue;
    for (size_t i = 0; i < 4; i++)
    {
        addressToken = addressTokens.getItemAtIndex(i);
        if (!isStringInteger(addressToken))
        {
            return false;
        }
        else
        {
            tokenValue = addressToken.toInt();
            if (tokenValue < 0 || tokenValue > 255)
            {
                return false;
            }
        }
    }

    return true;
}

inline bool RequestValidator::isLoginApiHeaderValid(vector<RequestHeader> headers)
{
    bool isUrlEncodedrequest, acceptsAppJson = false;

    for (RequestHeader header : headers)
    {
        if (header.key == "Content-Type" && header.value == R"(application/x-www-form-urlencoded)")
        {
            isUrlEncodedrequest = true;
        }
        else if (header.key == "Accept" && header.value == R"(application/json)")
        {
            acceptsAppJson = true;
        }
    }

    return isUrlEncodedrequest && acceptsAppJson;
}

inline bool RequestValidator::isApiHeaderValid(vector<RequestHeader> headers)
{
    bool hasApiKey, isUrlEncodedrequest, acceptsAppJson = false;

    for (RequestHeader header : headers)
    {
        if (header.key == "Content-Type" && header.value == R"(application/x-www-form-urlencoded)")
        {
            isUrlEncodedrequest = true;
        }
        else if (header.key == "Accept" && header.value == R"(application/json)")
        {
            acceptsAppJson = true;
        }
        else if (header.key == "Authorization" && header.key.substring(0, 6) == "Bearer|")
        {
            hasApiKey = true;
        }
    }

    return hasApiKey && isUrlEncodedrequest && acceptsAppJson;
}
