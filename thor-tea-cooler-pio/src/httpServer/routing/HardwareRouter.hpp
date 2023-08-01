#pragma once

#include "Router.hpp"
#include "httpServer/Controllers/HardwareController.hpp"

class HardwareRouter : public Router
{

private:
    AsyncWebServer *server;
    HardwareController *hwController;

    void onSwitchRelay();

public:
    HardwareRouter(AsyncWebServer *server_, TtcHardware *hw_, Authorization *auth_);
    ~HardwareRouter();
};

inline HardwareRouter::HardwareRouter(AsyncWebServer *server_, TtcHardware *hw_, Authorization *auth_)
{
    server = server_;

    hwController = new HardwareController(auth_, hw_);

    onSwitchRelay();
}

HardwareRouter::~HardwareRouter()
{
}

void HardwareRouter::onSwitchRelay()
{
    server->on("/switchRelay", HTTP_POST, [&](AsyncWebServerRequest *request)
               { hwController->switchRelay(request); });
}
