#include <Arduino.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "hardware/ttcHardware.hpp"

#include "main.hpp"

TtcHardware *hw;
HardwareConfiguration *hwConfig;

void setup()
{
    initializeSerial(115200, 1500);

    hwConfig = new HardwareConfiguration();

    loadHardwareConfig();
    initializeHardware();
    //  loadServerConfig();
    //  initializeServer();
    //  server.start();
    //  loadOscClientConfig();
    //  initializeOscClient();
}

void loop()
{
    delay(5000);
    hw->readTemperature();
    Serial.println("Temp: " + String(hw->getTemperature()));
}

void initializeSerial(int baudRate, int delayMs)
{
    Serial.begin(baudRate);
    delay(delayMs); //  let Serial boot up for proper boot sequence print
    Serial.println();
    Serial.println("----------------------------------------");
    Serial.println("-------   TTC v1.0 booting...   --------");
    Serial.println("----------------------------------------");
}

void loadHardwareConfig()
{
    Serial.println("BOOT: Loading hardware configuration...");
    hwConfig->setConfigFileName("/hardwareConfig.json");
    hwConfig->setConfigFileMaxSize(2048);

    hwConfig->initFileSystem();

    if (!hwConfig->loadFromDisk())
    {
        Serial.println("1, ERR - Hardware configuration load failed, restarting in 5 seconds...");
        delay(5000);
        ESP.restart();
    }
}

void initializeHardware()
{
    Serial.println("BOOT: Initializing hardware...");

    hw = new TtcHardware(hwConfig);

    if (hw->relay1->getState() != INITIALIZED || hw->relay2->getState() != INITIALIZED)
    {
        Serial.println("1, ERR - Hardware initialization failed, Relay initialization error. Restarting in 5 seconds...");
        delay(5000);
        ESP.restart();
    }
    else
    {
        Serial.println("0, OK - Relays initialized.");
        hw->relay1->off();
        hw->relay2->off();

        Serial.println("0, OK - Temperature sensor initialized.");

        Serial.println("0, OK - Hardware initialization successful.");
    }
}