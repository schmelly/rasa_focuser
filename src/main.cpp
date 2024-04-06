// platform includes
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncUDP.h>
#include <LittleFS.h>
#include <string.h>
#include <WiFi.h>

// web includes
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>

// utility includes
#include "debug.h"

// focuser includes
#include "focuser_control.h"
#include "request_handling.h"
#include "stepper_control.h"

// web objects
AsyncWebServer server(80);
AsyncUDP udp;
DNSServer dns;
AsyncWiFiManager wifiManager(&server, &dns);

// alpaca objects
uint32_t transactionId = 0;
const int32_t startPosition = (int32_t)(MAX_STEP / 2.0);
int32_t targetPosition = startPosition;
bool calibratingRMS = false;

void notFound(AsyncWebServerRequest *request)
{
  // Handle Unknown Request
  Serial.print("Unknown request: ");
  Serial.print(request->methodToString());
  Serial.print(" ");
  Serial.print(request->url());
  Serial.println("");
  request->send(404);
}

void setup()
{
  Serial.begin(115200); // Init serial port and set baudrate
  Serial2.begin(115200);
  while (!(Serial && Serial2))
    ; // Wait for serial port to connect
  Serial.println("\nStart...");

  setupStepperControl();

  LittleFS.begin();

  // setup worker core

  setupFocuserControl();

  // setup wifi

  wifiManager.setTimeout(180);
  if (!wifiManager.autoConnect("AutoConnectAP", "autoconnectapsecret"))
  {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    // reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }
  // if you get here you have connected to the WiFi
  //  wifiConnected = true;
  Serial.println("connected...yeey :)");

  if (udp.listen(32227))
  {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet)
                 {
      auto *expected = "alpacadiscovery1";
      if(packet.isBroadcast() && strcmp(expected, (char*)packet.data()) == 0) {
        Serial.println("received Alpaca broadcast, responding...");
        packet.printf("{\"AlpacaPort\":80}");
      } });
  }

  // handle management requests
  server.on("/management/apiversions", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::management_apiversions, HTTP_GET>(request); });

  server.on("/management/v1/description", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::management_v1_description, HTTP_GET>(request); });

  server.on("/management/v1/configureddevices", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::management_v1_configureddevices, HTTP_GET>(request); });

  // handle common device requests
  server.on("/api/v1/focuser/0/connected", HTTP_PUT, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::common_connected, HTTP_PUT>(request); });

  server.on("/api/v1/focuser/0/connected", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::common_connected, HTTP_GET>(request); });

  server.on("/api/v1/focuser/0/description", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::common_description, HTTP_GET>(request); });

  server.on("/api/v1/focuser/0/driverinfo", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::common_driverinfo, HTTP_GET>(request); });

  server.on("/api/v1/focuser/0/driverversion", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::common_driverversion, HTTP_GET>(request); });

  server.on("/api/v1/focuser/0/interfaceversion", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::common_interfaceversion, HTTP_GET>(request); });

  server.on("/api/v1/focuser/0/name", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::common_name, HTTP_GET>(request); });

  server.on("/api/v1/focuser/0/supportedactions", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::common_supportedactions, HTTP_GET>(request); });

  // handle focuser specific requests
  server.on("/api/v1/focuser/0/absolute", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::focuser_absolute, HTTP_GET>(request); });

  server.on("/api/v1/focuser/0/halt", HTTP_PUT, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::focuser_halt, HTTP_PUT>(request); });

  server.on("/api/v1/focuser/0/ismoving", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::focuser_ismoving, HTTP_GET>(request); });

  server.on("/api/v1/focuser/0/maxstep", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::focuser_maxstep, HTTP_GET>(request); });

  server.on("/api/v1/focuser/0/maxincrement", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::focuser_maxincrement, HTTP_GET>(request); });

  server.on("/api/v1/focuser/0/move", HTTP_PUT, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::focuser_move, HTTP_PUT>(request); });

  server.on("/api/v1/focuser/0/position", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::focuser_position, HTTP_GET>(request); });

  server.on("/api/v1/focuser/0/stepsize", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::focuser_stepsize, HTTP_GET>(request); });

  server.on("/api/v1/focuser/0/tempcomp", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::focuser_tempcomp, HTTP_GET>(request); });

  server.on("/api/v1/focuser/0/tempcomp", HTTP_PUT, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::focuser_tempcomp, HTTP_PUT>(request); });

  server.on("/api/v1/focuser/0/tempcompavailable", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::focuser_tempcompavailable, HTTP_GET>(request); });

  server.on("/api/v1/focuser/0/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
            { handleRequest<AlpacaRequestType::focuser_temperature, HTTP_GET>(request); });

  server.on("/reset_position", HTTP_GET, [](AsyncWebServerRequest *request)
            { resetPosition(request); request->send(200, "text/plain", "ok"); });

  server.on("/calibrate_rms", HTTP_GET, [](AsyncWebServerRequest *request)
            { beginCalibrateRMS(); request->send(200, "text/plain", "ok"); });

  server.on("/setup", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "hello world"); });

  server.on("/setup/v1/focuser/0/setup", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "hello world"); });

  server.on("/reset_wifi", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              wifiManager.resetSettings();
              request->send(200, "text/plain", "wifi resetted, restarting");
              ESP.restart();
            });

  server.on("/reset_esp", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              request->send(200, "text/plain", "esp restarting");
              ESP.restart();
            });

  server.onNotFound(notFound);

  server.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
}