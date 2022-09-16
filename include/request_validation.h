#ifndef REQUEST_VALIDATION_H
#define REQUEST_VALIDATION_H

#include <charconv>
#include <ESPAsyncWebServer.h>
#include <limits>
#include <type_traits>

#include "request.h"

struct ValidationResult
{
    bool isValid;
    const String errorMsg;
};

// common error messages
extern const String IS;
extern const String MISSING;
extern const String OUT_OF_RANGE;
extern const String INVALID;

// parameter names
extern const String VALUE;
extern const String ERROR_NUMMER;
extern const String ERROR_MESSAGE;
extern const String CLIENT_ID;
extern const String CLIENT_TX_ID;
extern const String CONNECTED;
extern const String POSITION;

extern const ValidationResult RESULT_OK;

ValidationResult validateCommonRequestSection(AsyncWebServerRequest *request);

template <typename T,
          typename std::enable_if<std::is_same<T, bool>::value, bool>::type = true>
T getOrDefaultRequestParam(AsyncWebServerRequest *request, String param, T defaultValue = false) {

    if(!request->hasParam(param, true)) {
        return defaultValue;
    }
    
    auto str = request->getParam(param, true)->value();

    if (str == "0" ||  str.equalsIgnoreCase("false"))
    {
        return false;
    } else {
        return true;
    }
}

template <typename T,
          typename std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value, bool>::type = true>
T getOrDefaultRequestParam(AsyncWebServerRequest *request, String param, T defaultValue = 0, bool post = true) {

    if(!request->hasParam(param, post)) {
        return defaultValue;
    }
    
    auto str = request->getParam(param, post)->value();
    T result{};
    auto [ptr, ec]{std::from_chars(std::string(str.c_str()).data(), std::string(str.c_str()).data() + std::string(str.c_str()).size(), result)};

    return result;
}

template <typename T,
          typename std::enable_if<!std::is_arithmetic<T>::value, bool>::type = true>
T getOrDefaultRequestParam(AsyncWebServerRequest *request, String param, T defaultValue = "", bool post = true) {

    if(!request->hasParam(param, post)) {
        return defaultValue;
    }

    return request->getParam(param, post)->value();
}

template <typename T,
          typename std::enable_if<std::is_same<T, bool>::value, bool>::type = true>
ValidationResult validateRequestParam(AsyncWebServerRequest *request, String param, bool required = true, bool post = true)
{
    if (required && !request->hasParam(param, post))
    {
        return {false, param + IS + MISSING};
    };

    if (!required && !request->hasParam(param, post))
    {
        return RESULT_OK;
    }

    auto str = request->getParam(param, post)->value();

    if (str != "0" && str != "1" && !str.equalsIgnoreCase("true") && !str.equalsIgnoreCase("false"))
    {
        return {false, param + IS + INVALID};
    }

    return RESULT_OK;
}

template <typename T,
          typename std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value, bool>::type = true>
ValidationResult validateRequestParam(AsyncWebServerRequest *request, String param, bool required = true, bool post = true)
{
    if (required && !request->hasParam(param, post))
    {
        return {false, param + IS + MISSING};
    };

    if (!required && !request->hasParam(param, post))
    {
        return RESULT_OK;
    }

    auto str = request->getParam(param, post)->value();
    T result{};
    auto [ptr, ec]{std::from_chars(std::string(str.c_str()).data(), std::string(str.c_str()).data() + std::string(str.c_str()).size(), result)};

    if (ec == std::errc::invalid_argument)
    {
        return {false, param + IS + INVALID};
    }
    else if (ec == std::errc::result_out_of_range)
    {
        return {false, param + IS + OUT_OF_RANGE};
    }

    return RESULT_OK;
}

template <typename T,
          typename std::enable_if<!std::is_arithmetic<T>::value, bool>::type = true>
ValidationResult validateRequestParam(AsyncWebServerRequest *request, String param, bool required = true, bool post = true)
{
    if (required && !request->hasParam(param, post))
    {
        return {false, param + IS + MISSING};
    };

    return RESULT_OK;
}

template <AlpacaRequestType T, WebRequestMethod M>
ValidationResult validateRequestSection(AsyncWebServerRequest *request);
// {
//     return RESULT_OK;
// }

template <AlpacaRequestType T, WebRequestMethod M>
ValidationResult validateRequest(AsyncWebServerRequest *request)
{
    ValidationResult commonReqSectionResult = validateCommonRequestSection(request);
    if (!commonReqSectionResult.isValid)
    {
        return commonReqSectionResult;
    }
    return validateRequestSection<T, M>(request);
}

#endif // REQUEST_VALIDATION_H
