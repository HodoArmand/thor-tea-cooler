#pragma once

#include "Router.hpp"
#include "httpServer/Controllers/ServerSideEventController.hpp"

class ServerSideEventRouter : public Router
{
private:
    ServerSideEventController *sseController;

public:
    ServerSideEventRouter(TtcHardware *hw_, AsyncWebServer *server_, String eventPath);
    ~ServerSideEventRouter();

    void sendPing();
    void sendTeaState();
};

ServerSideEventRouter::ServerSideEventRouter(TtcHardware *hw_, AsyncWebServer *server_, String eventPath)
{
    sseController = new ServerSideEventController(hw_, server_, eventPath);
    sseController->handleOnConnect();
}

ServerSideEventRouter::~ServerSideEventRouter()
{
}

inline void ServerSideEventRouter::sendPing()
{
    sseController->ping();
}

inline void ServerSideEventRouter::sendTeaState()
{
    sseController->sendTeaState();
}
