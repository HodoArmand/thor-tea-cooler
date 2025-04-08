#pragma once

#include <Arduino.h>

#include <WiFi.h>
#include "../lib/AsyncTCPQuemod/AsyncTCP.h"
#define PSY_ENABLE_SSL true
#define CONFIG_ESP_HTTPS_SERVER_ENABLE
#include <PsychicHttpsServer.h>

#include "hardware/ttcHardware.hpp"
#include "network/wifiNetworkAdapter.hpp"
#include "httpServer/httpApiServer.hpp"
#include "Timing.hpp"

void initializeTimers();
void initializeBootFeedback();

void initializeSerial(int baudRate, int delayMs);
void loadHardwareConfig();
void initializeHardware();
void loadNetworkConfig();
void connectToWifi();
void loadServerConfig();
void initializeApiServer();
// void loadOscClientConfig();
// void initializeOscClient();

void setBootFinishedFeedback();

// TODO: 1.1: add OSC interface