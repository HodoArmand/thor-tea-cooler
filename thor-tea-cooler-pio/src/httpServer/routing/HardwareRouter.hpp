#pragma once

#include "Router.hpp"
#include "httpServer/Controllers/HardwareController.hpp"
#include "httpServer/routing/ServerSideEventRouter.hpp"

class HardwareRouter : public Router
{

private:
    PsychicHttpServer *server;
    HardwareController *hwController;
    ServerSideEventRouter *sse;

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
    HardwareRouter(PsychicHttpServer *server_, TtcHardware *hw_, Authorization *auth_, ServerSideEventRouter *sse_);
    ~HardwareRouter();
};

inline HardwareRouter::HardwareRouter(PsychicHttpServer *server_, TtcHardware *hw_, Authorization *auth_, ServerSideEventRouter *sse_)
{
    server = server_;

    hwController = new HardwareController(auth_, hw_, sse_);

    sse = sse_;

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
    server->on("/getHardwareState", HTTP_GET, [&](PsychicRequest *request)
               { return hwController->getHardwareState(request); });
}

void HardwareRouter::onSwitchRelay()
{
    server->on("/switchRelay", HTTP_POST, [&](PsychicRequest *request, JsonVariant &json)
               { return hwController->switchRelay(request, json); });
}

void HardwareRouter::onSetRelays()
{
    server->on("/setRelays", HTTP_POST, [&](PsychicRequest *request, JsonVariant &json)
               { return hwController->setRelays(request, json); });
}

inline void HardwareRouter::onSetModeManual()
{
    server->on("/setModeManual", HTTP_POST, [&](PsychicRequest *request)
               { return hwController->setModeManual(request); });
}

inline void HardwareRouter::onSetModeAuto()
{
    server->on("/setModeAuto", HTTP_POST, [&](PsychicRequest *request)
               { return hwController->setModeAuto(request); });
}

inline void HardwareRouter::onSetTargetTemperature()
{
    server->on("/setTargetTemperature", HTTP_POST, [&](PsychicRequest *request, JsonVariant &json)
               { return hwController->setTargetTemperature(request, json); });
}

inline void HardwareRouter::onStartAutoCooling()
{
    server->on("/startAutoCooling", HTTP_POST, [&](PsychicRequest *request)
               { return hwController->startAutoCooling(request); });
}

inline void HardwareRouter::onStopAutoCooling()
{
    server->on("/stopAutoCooling", HTTP_POST, [&](PsychicRequest *request)
               { return hwController->stopAutoCooling(request); });
}

inline void HardwareRouter::onRestartMcu()
{
    server->on("/restartMcu", HTTP_POST, [&](PsychicRequest *request)
               { return hwController->restartMcu(request); });
}
