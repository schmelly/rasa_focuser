#ifndef REQUEST_HANDLING_H
#define REQUEST_HANDLING_H

#include <ArduinoJson.h>

#include "debug.h"
#include "request_validation.h"
#include "state_handling.h"
#include "stepper_control.h"

extern uint32_t transactionId;
extern int32_t position;

// basic responses

DynamicJsonDocument simpleEmptyResponse();

DynamicJsonDocument notImplementedResponse();

DynamicJsonDocument notConnectedResponse();

template <AlpacaRequestType T, WebRequestMethod M>
DynamicJsonDocument prepareResponse();

// request processing

template <AlpacaRequestType T, WebRequestMethod M>
Result processRequest(AsyncWebServerRequest *request);

template <>
Result processRequest<AlpacaRequestType::common_connected, HTTP_PUT>(AsyncWebServerRequest *request);
// {
//     auto connected = getOrDefaultRequestParam<bool>(request, CONNECTED);
//     return submitEvent<EventType::CONNECT>((int32_t)connected);
// }

template <>
Result processRequest<AlpacaRequestType::focuser_halt, HTTP_PUT>(AsyncWebServerRequest *request);
// {
//     return submitEvent<EventType::HALT>((int32_t)0);
// }

template <>
Result processRequest<AlpacaRequestType::focuser_move, HTTP_PUT>(AsyncWebServerRequest *request);
// {
//     auto position = getOrDefaultRequestParam<int32_t>(request, POSITION);
//     return submitEvent<EventType::MOVE>(position);
// }

template <AlpacaRequestType T, WebRequestMethod M>
Result processRequest(AsyncWebServerRequest *request)
{
    serialPrintln("processRequest<*,*>(request.url=%s)", request->url().c_str());
    return Result::OK;
}

template <typename T>
DynamicJsonDocument simpleValueResponse(T value)
{
    DynamicJsonDocument jsonResponse(64);
    jsonResponse[VALUE] = value;
    jsonResponse[ERROR_NUMMER] = 0;
    jsonResponse[ERROR_MESSAGE] = "";
    return jsonResponse;
}

template <AlpacaRequestType T, WebRequestMethod M>
void handleRequest(AsyncWebServerRequest *request)
{
    auto validationResult = validateRequest<T, M>(request);
    if (!validationResult.isValid)
    {
        request->send(400, "text/plain", validationResult.errorMsg.c_str());
        return;
    }

    auto clientTxId = getOrDefaultRequestParam<uint32_t>(request, CLIENT_TX_ID);

    Result r = processRequest<T, M>(request);
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument jsonResponse(0);
    if (Result::NOT_CONNECTED == r)
    {
        jsonResponse = notConnectedResponse();
    }
    else
    {
        jsonResponse = prepareResponse<T, M>();
        jsonResponse["ClientTransactionID"] = clientTxId;
        jsonResponse["ServerTransactionID"] = transactionId++;
    }
    serializeJson(jsonResponse, *response);
    request->send(response);
}

void rotateAndCenter(AsyncWebServerRequest *request);

#endif // REQUEST_HANDLING_H