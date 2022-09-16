#include "request_validation.h"

// common error messages
const String IS = " is ";
const String MISSING = "missing";
const String OUT_OF_RANGE = "out of range";
const String INVALID = "invalid";

// parameter names
const String VALUE = "Value";
const String ERROR_NUMMER = "ErrorNumber";
const String ERROR_MESSAGE = "ErrorMessage";
const String CLIENT_ID = "ClientID";
const String CLIENT_TX_ID = "ClientTransactionID";
const String CONNECTED = "Connected";
const String POSITION = "Position";

const ValidationResult RESULT_OK = {true, ""};

ValidationResult validateCommonRequestSection(AsyncWebServerRequest *request)
{
    auto clientIdValid = validateRequestParam<uint32_t>(request, CLIENT_ID, false);
    if (!clientIdValid.isValid)
    {
        return clientIdValid;
    }

    auto clientTxIdValid = validateRequestParam<uint32_t>(request, CLIENT_TX_ID, false);
    if (!clientTxIdValid.isValid)
    {
        return clientTxIdValid;
    }

    return RESULT_OK;
}

// get requests

template <>
ValidationResult validateRequestSection<AlpacaRequestType::management_apiversions, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::management_v1_description, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::management_v1_configureddevices, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::common_connected, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::common_description, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::common_driverinfo, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::common_driverversion, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::common_interfaceversion, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::common_name, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::common_supportedactions, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::focuser_absolute, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::focuser_ismoving, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::focuser_maxstep, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::focuser_maxincrement, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::focuser_position, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::focuser_stepsize, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::focuser_tempcomp, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::focuser_tempcompavailable, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::focuser_temperature, HTTP_GET>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

// put requests

template <>
ValidationResult validateRequestSection<AlpacaRequestType::common_connected, HTTP_PUT>(AsyncWebServerRequest *request)
{
    return validateRequestParam<bool>(request, CONNECTED);
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::focuser_halt, HTTP_PUT>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::focuser_move, HTTP_PUT>(AsyncWebServerRequest *request)
{
    return validateRequestParam<int32_t>(request, POSITION);
}

template <>
ValidationResult validateRequestSection<AlpacaRequestType::focuser_tempcomp, HTTP_PUT>(AsyncWebServerRequest *request)
{
    return RESULT_OK;
}
