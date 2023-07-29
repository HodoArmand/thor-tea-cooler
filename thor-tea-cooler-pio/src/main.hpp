#pragma once

void initializeSerial(int baudRate, int delayMs);
void loadHardwareConfig();
void initializeHardware();
void loadNetworkConfig();
void connectToWifi();
// void loadServerConfig();
// void initializeApiServer();
// void server.start();
// void loadOscClientConfig();
// void initializeOscClient();