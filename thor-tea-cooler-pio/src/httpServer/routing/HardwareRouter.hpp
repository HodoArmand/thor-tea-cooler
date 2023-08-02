#pragma once

#include "Router.hpp"
#include "httpServer/Controllers/HardwareController.hpp"

class HardwareRouter : public Router
{

private:
    AsyncWebServer *server;
    HardwareController *hwController;

    void onGetHardwareState();

    void onSwitchRelay();
    void onSetRelays();
    void onSetModeManual();
    void onSetModeAuto();
    void onSetTargetTemperature();
    void onStartAutoCooling();
    void onStopAutoCooling();

    void onRestartMcu();

public:
    HardwareRouter(AsyncWebServer *server_, TtcHardware *hw_, Authorization *auth_);
    ~HardwareRouter();
};

inline HardwareRouter::HardwareRouter(AsyncWebServer *server_, TtcHardware *hw_, Authorization *auth_)
{
    server = server_;

    hwController = new HardwareController(auth_, hw_);

    onGetHardwareState();

    onSwitchRelay();
    onSetRelays();
    onSetModeManual();
    onSetModeAuto();
    onSetTargetTemperature();
    onStartAutoCooling();
    onStopAutoCooling();

    onRestartMcu();
}

HardwareRouter::~HardwareRouter()
{
}

void HardwareRouter::onGetHardwareState()
{
    server->on("/getHardwareState", HTTP_GET, [&](AsyncWebServerRequest *request)
               { hwController->getHardwareState(request); });
}

void HardwareRouter::onSwitchRelay()
{
    server->on("/switchRelay", HTTP_POST, [&](AsyncWebServerRequest *request)
               { hwController->switchRelay(request); });
}

void HardwareRouter::onSetRelays()
{
    server->on("/setRelays", HTTP_POST, [&](AsyncWebServerRequest *request)
               { hwController->setRelays(request); });
}

inline void HardwareRouter::onSetModeManual()
{
    server->on("/setModeManual", HTTP_POST, [&](AsyncWebServerRequest *request)
               { hwController->setModeManual(request); });
}

inline void HardwareRouter::onSetModeAuto()
{
    server->on("/setModeAuto", HTTP_POST, [&](AsyncWebServerRequest *request)
               { hwController->setModeAuto(request); });
}

inline void HardwareRouter::onSetTargetTemperature()
{
    server->on("/setTargetTemperature", HTTP_POST, [&](AsyncWebServerRequest *request)
               { hwController->setTargetTemperature(request); });
}

inline void HardwareRouter::onStartAutoCooling()
{
    server->on("/startAutoCooling", HTTP_POST, [&](AsyncWebServerRequest *request)
               { hwController->startAutoCooling(request); });
}

inline void HardwareRouter::onStopAutoCooling()
{
    server->on("/stopAutoCooling", HTTP_POST, [&](AsyncWebServerRequest *request)
               { hwController->stopAutoCooling(request); });
}

inline void HardwareRouter::onRestartMcu()
{
    server->on("/restartMcu", HTTP_POST, [&](AsyncWebServerRequest *request)
               { hwController->restartMcu(request); });
}
