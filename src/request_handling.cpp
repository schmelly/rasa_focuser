#include "request_handling.h"

DynamicJsonDocument simpleEmptyResponse()
{
    DynamicJsonDocument jsonResponse(64);
    jsonResponse[ERROR_NUMMER] = 0;
    jsonResponse[ERROR_MESSAGE] = "";
    return jsonResponse;
}

DynamicJsonDocument notImplementedResponse()
{
    DynamicJsonDocument jsonResponse(64);
    jsonResponse[ERROR_NUMMER] = 0x400;
    jsonResponse[ERROR_MESSAGE] = "not implemented";
    return jsonResponse;
}

DynamicJsonDocument notConnectedResponse()
{
    DynamicJsonDocument jsonResponse(64);
    jsonResponse[ERROR_NUMMER] = 0x407;
    jsonResponse[ERROR_MESSAGE] = "not connected";
    return jsonResponse;
}

// management api responses

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::management_apiversions, WebRequestMethod::HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::management_apiversions, WebRequestMethod::HTTP_GET>()");
    DynamicJsonDocument jsonResponse(192);
    JsonArray value = jsonResponse.createNestedArray("Value");
    value.add(1);
    value.add(2);
    value.add(3);
    value.add(4);
    value.add(5);
    value.add(6);
    value.add(7);
    return jsonResponse;
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::management_v1_configureddevices, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::management_v1_configureddevices, HTTP_GET>()");
    DynamicJsonDocument jsonResponse(256);
    JsonObject value = jsonResponse["Value"].createNestedObject();
    value["DeviceName"] = "RASA Focuser";
    value["DeviceType"] = "Focuser";
    value["DeviceNumber"] = 0;
    value["UniqueID"] = "13DCD76F-7771-4E12-8A5C-1AC1F9EA658B";
    return jsonResponse;
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::management_v1_description, HTTP_GET>()
{
    serialPrintln("repareResponse<AlpacaRequestType::management_v1_description, HTTP_GET>()");
    DynamicJsonDocument jsonResponse(192);
    JsonObject value = jsonResponse.createNestedObject("Value");
    value["ServerName"] = "RASA Focuser";
    value["Manufacturer"] = "schmelly";
    value["ManufacturerVersion"] = "v1.0.0";
    value["Location"] = "Paderborn, Germany";
    return jsonResponse;
}

// common api responses

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::common_connected, HTTP_PUT>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::common_connected, HTTP_PUT>()");
    return simpleEmptyResponse();
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::common_connected, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::common_connected, HTTP_GET>()");
    return simpleValueResponse(curState == State::CONNECTED ? false : true);
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::common_description, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::common_description, HTTP_GET>()");
    return simpleValueResponse("schmelly's diy focuser");
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::common_driverinfo, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::common_driverinfo, HTTP_GET>()");
    return simpleValueResponse("schmelly's diy focuser driver");
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::common_driverversion, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::common_driverversion, HTTP_GET>()");
    return simpleValueResponse("1.0");
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::common_interfaceversion, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::common_interfaceversion, HTTP_GET>()");
    return simpleValueResponse(7);
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::common_name, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::common_name, HTTP_GET>()");
    return simpleValueResponse("RASA Focuser");
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::common_supportedactions, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::common_supportedactions, HTTP_GET>()");
    DynamicJsonDocument jsonResponse(64);
    jsonResponse.createNestedArray(VALUE);
    jsonResponse[ERROR_NUMMER] = 0;
    jsonResponse[ERROR_MESSAGE] = "";
    return jsonResponse;
}

// focuser api responses

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::focuser_absolute, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::focuser_absolute, HTTP_GET>()");
    return simpleValueResponse(true);
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::focuser_halt, HTTP_PUT>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::focuser_halt, HTTP_PUT>()");
    return simpleEmptyResponse();
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::focuser_ismoving, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::focuser_ismoving, HTTP_GET>()");
    return simpleValueResponse(curState == State::MOVING ? true : false);
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::focuser_maxstep, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::focuser_maxstep, HTTP_GET>()");
    return simpleValueResponse(MAX_STEP);
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::focuser_maxincrement, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::focuser_maxincrement, HTTP_GET>()");
    return simpleValueResponse(MAX_INCREMENT);
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::focuser_move, HTTP_PUT>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::focuser_move, HTTP_PUT>()");
    return simpleEmptyResponse();
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::focuser_position, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::focuser_position, HTTP_GET>()");
    return simpleValueResponse(position);
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::focuser_stepsize, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::focuser_stepsize, HTTP_GET>()");
    auto stepSize = STEP_SIZE;
    return simpleValueResponse(stepSize);
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::focuser_tempcomp, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::focuser_tempcomp, HTTP_GET>()");
    return simpleValueResponse(false);
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::focuser_tempcomp, HTTP_PUT>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::focuser_tempcomp, HTTP_PUT>()");
    return notImplementedResponse();
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::focuser_tempcompavailable, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::focuser_tempcompavailable, HTTP_GET>()");
    return simpleValueResponse(false);
}

template <>
DynamicJsonDocument prepareResponse<AlpacaRequestType::focuser_temperature, HTTP_GET>()
{
    serialPrintln("prepareResponse<AlpacaRequestType::focuser_temperature, HTTP_GET>()");
    return notImplementedResponse();
}

// request processing
template <>
Result processRequest<AlpacaRequestType::common_connected, HTTP_PUT>(AsyncWebServerRequest *request)
{
    auto connected = getOrDefaultRequestParam<bool>(request, CONNECTED);
    Result result = submitEvent<EventType::CONNECT>((int32_t)connected);
    serialPrintln("processRequest<AlpacaRequestType::common_connected, HTTP_PUT>(connected=%d)=%d", connected, result);
    return result;
}

template <>
Result processRequest<AlpacaRequestType::focuser_halt, HTTP_PUT>(AsyncWebServerRequest *request)
{
    Result result = submitEvent<EventType::HALT>((int32_t)0);
    serialPrintln("processRequest<AlpacaRequestType::focuser_halt, HTTP_PUT>()=%d", result);
    return result;
}

template <>
Result processRequest<AlpacaRequestType::focuser_move, HTTP_PUT>(AsyncWebServerRequest *request)
{
    auto position = getOrDefaultRequestParam<int32_t>(request, POSITION);

    if (position < 0)
    {
        position = 0;
    }
    else if (position > MAX_STEP)
    {
        position = MAX_STEP;
    }

    Result result = submitEvent<EventType::MOVE>(position);
    serialPrintln("processRequest<AlpacaRequestType::focuser_move, HTTP_PUT>(position=%d)=%d", position, result);
    return result;
}

void rotateAndCenter(AsyncWebServerRequest *request)
{
    auto angle = getOrDefaultRequestParam<int32_t>(request, "angle", 0, false);
    int32_t steps = (int32_t)STEPS_PER_ANGLE(angle);
    step(abs(steps), angle >= 0);
    position = (int32_t)(MAX_STEP / 2.0);
    targetPosition = (int32_t)(MAX_STEP / 2.0);
}
