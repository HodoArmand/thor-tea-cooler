#pragma once

#include <math.h>

#include "relay.hpp"
#include "hardware/hardwareConfiguration.hpp"
#include "hardware/ds18B20-universal-driver.hpp"

using namespace std;

enum TtcHardwareMode
{
    manual,
    autoReady,
    autoCooling,
    autoFinished
};

class TtcHardware
{
private:
    OneWire oneWire;
    DsDriver ds;

    bool relayState1;
    bool relayState2;

    float temperature;
    float targetTemperature;

    TtcHardwareMode mode = manual;

    float roundFloatToDecimals(float num, int decimals);

    vector<String> validModes = {"manual", "autoReady", "autoCooling", "autoFinished"};

public:
    TtcHardware(HardwareConfiguration *hwConfig);
    Relay *relay1;
    Relay *relay2;

    vector<String> getValidModes() const { return validModes; }

    TtcHardwareMode getMode() const { return mode; }
    String getModeAsString();
    TtcHardwareMode stringToTtcHardwareMode(String mode_);
    void setMode(const TtcHardwareMode &mode_) { mode = mode_; }

    ~TtcHardware();

    void readTemperature();

    float getTargetTemperature() const { return targetTemperature; }
    void setTargetTemperature(float targetTemperature_) { targetTemperature = targetTemperature_; }

    float getTemperature() const { return temperature; }

    void setRelays(bool rel1State, bool rel2State);
    bool switchRelay(int relayNumber);

    void setModeManual();
    void setModeAuto();
    void startCooling();
    void stopCooling();

    void readHardwareState();
    void logHardwareState();

    String getHardwareStateAsJsonString();
};

inline float TtcHardware::roundFloatToDecimals(float number, int decimals)
{
    int decimalMultiplier = (int)pow10f(decimals);
    float decimalDivider = pow10f(decimals);
    return ((int)(number * decimalMultiplier + .5) / decimalDivider);
}

TtcHardware::TtcHardware(HardwareConfiguration *hwConfig)
{
    relay1 = new Relay(hwConfig->getRelayIoPin1());
    relay2 = new Relay(hwConfig->getRelayIoPin2());
    oneWire = OneWire(hwConfig->getOneWireIoPin());
    ds = DsDriver(hwConfig->getTemperatureSensorOffsetCelsius(), oneWire, hwConfig->getDebugMode());
    setTargetTemperature(hwConfig->getTemperatureTargetDefault());
    ds.scanSensors();
}

TtcHardware::~TtcHardware()
{
}

String TtcHardware::getModeAsString()
{
    switch (mode)
    {
    case manual:
        return String("manual");
        break;

    case autoReady:
        return String("autoReady");
        break;

    case autoCooling:
        return String("autoCooling");
        break;

    case autoFinished:
        return String("autoFinished");
        break;

    default:
        return String("manual");
        break;
    }
}

TtcHardwareMode TtcHardware::stringToTtcHardwareMode(String mode_)
{
    if (mode_ == "manual")
    {
        return manual;
    }
    else if (mode_ == "autoReady")
    {
        return autoReady;
    }
    else if (mode_ == "autoCooling")
    {
        return autoCooling;
    }
    else if (mode_ == "autoFinished")
    {
        return autoFinished;
    }
    else
    {
        return manual;
    }
}

void TtcHardware::readTemperature()
{
    temperature = roundFloatToDecimals(ds.readSensortemperatureInCelsiusByIndex(0), 2);
}

void TtcHardware::setRelays(bool rel1State, bool rel2State)
{
    if (rel1State)
    {
        relay1->on();
    }
    else
    {
        relay1->off();
    }
    if (rel2State)
    {
        relay2->on();
    }
    else
    {
        relay2->off();
    }
}

inline bool TtcHardware::switchRelay(int relayNumber)
{
    if (relayNumber != 1 && relayNumber != 2)
    {
        return false;
    }
    else
    {
        if (relayNumber == 1)
        {
            relay1->toggle();
        }
        else if (relayNumber == 2)
        {
            relay2->toggle();
        }
        return true;
    }
}

void TtcHardware::setModeManual()
{
    setRelays(false, false);
    setMode(manual);
}
void TtcHardware::setModeAuto()
{
    setRelays(false, false);
    setMode(autoReady);
}
void TtcHardware::startCooling()
{
    setRelays(true, true);
    setMode(autoCooling);
}
void TtcHardware::stopCooling()
{
    setRelays(false, false);
    setMode(autoFinished);
}

void TtcHardware::readHardwareState()
{
    relayState1 = relay1->isOn();
    relayState2 = relay2->isOn();

    readTemperature();
}

void TtcHardware::logHardwareState()
{
    Serial.println("\nTTC state:\n");
    Serial.println("Relay 1 state: " + String(relay1->getState()) + " on/off: " + relay1->isOn());
    Serial.println("Relay 2 state: " + String(relay2->getState()) + " on/off: " + relay2->isOn());
    Serial.println("Temperature: " + String(getTemperature()) + " C");
    Serial.println("Temperature target: " + String(getTargetTemperature()) + " C");
    Serial.println("Mode: " + getModeAsString());
}

String TtcHardware::getHardwareStateAsJsonString()
{
    DynamicJsonDocument json(2048);
    String serializedJson;
    json["relay1"] = relay1->isOn();
    json["relay2"] = relay2->isOn();
    json["temperature"] = serialized(String(getTemperature(), 2));
    json["targetTemperature"] = serialized(String(getTargetTemperature(), 2));
    json["mode"] = getModeAsString();

    serializeJsonPretty(json, serializedJson);

    json.clear();

    return serializedJson;
}