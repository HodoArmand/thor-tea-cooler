#pragma once

#include <Arduino.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "hardware/ttcHardware.hpp"
#include "network/wifiNetworkAdapter.hpp"
#include "httpServer/httpApiServer.hpp"

void initializeSerial(int baudRate, int delayMs);
void loadHardwareConfig();
void initializeHardware();
void loadNetworkConfig();
void connectToWifi();
void loadServerConfig();
void initializeApiServer();
// void loadOscClientConfig();
// void initializeOscClient();

// TODO: add OSC interface