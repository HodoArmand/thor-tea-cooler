#pragma once

#include <Arduino.h>
#include <PsychicHttp.h>
#define CONFIG_ESP_HTTPS_SERVER_ENABLE
#define PSY_ENABLE_SSL true
#include <PsychicHttpsServer.h>

#include "httpServer/Controllers/Controller.hpp"
#include "hardware/ttcHardware.hpp"

class ServerSideEventController : public Controller
{
private:
    TtcHardware *hw;
    PsychicHttpsServer *server;
    PsychicEventSource events;

public:
    ServerSideEventController(TtcHardware *hw_, PsychicHttpsServer *server_, String eventPath);
    ~ServerSideEventController();

    void handleOnConnect();
    void ping();
    void sendEvent(String serializedJsonData, String eventName);
    void sendTeaState();
};

ServerSideEventController::ServerSideEventController(TtcHardware *hw_, PsychicHttpsServer *server_, String eventPath)
{
    this->hw = hw_;
    this->server = server_;
    this->events = PsychicEventSource();

    events.onOpen([](PsychicEventSourceClient *client)
                  {
        if (client->lastId())
        {
            Serial.printf("SSE client connected, ID: %u\n", client->lastId());
                }
        client->send("SSE hello!", "ping", millis(), 10000); });
    server->on(eventPath.c_str(), &events);
}

ServerSideEventController::~ServerSideEventController()
{
}

inline void ServerSideEventController::ping()
{
    events.send("ping", "ping", millis());
}

inline void ServerSideEventController::sendEvent(String serializedJsonData, String eventName)
{
    events.send(String(serializedJsonData).c_str(), eventName.c_str(), millis());
}

void ServerSideEventController::sendTeaState()
{
    sendEvent(hw->getHardwareStateAsJsonString(), "teaState");
}