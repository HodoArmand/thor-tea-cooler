#pragma once

#include "Router.hpp"
#include "httpServer/Controllers/ConfigurationController.hpp"

class ConfigurationRouter : public Router
{

private:
    AsyncWebServer *server;

    Authorization *auth;
    HardwareConfiguration *hwConfig;
    NetworkConfiguration *networkConfig;
    ServerConfiguration *serverConfig;
    ConfigurationController *configController;

public:
    ConfigurationRouter(AsyncWebServer *server_, Authorization *auth_, HardwareConfiguration *hwConfig_, NetworkConfiguration *networkConfig_, ServerConfiguration *serverConfig_);
    ~ConfigurationRouter();

    void onGetHardwareConfig();
    void onSetHardwareConfig();

    void onGetNetworkConfig();
    void onSetNetworkConfig();

    void onGetServerConfig();
    void onSetServerConfig();
};

inline ConfigurationRouter::ConfigurationRouter(AsyncWebServer *server_, Authorization *auth_, HardwareConfiguration *hwConfig_, NetworkConfiguration *networkConfig_, ServerConfiguration *serverConfig_)
{
    server = server_;
    auth = auth_;
    hwConfig = hwConfig_;
    networkConfig = networkConfig_;
    serverConfig = serverConfig_;

    configController = new ConfigurationController(auth, hwConfig, networkConfig, serverConfig);

    onGetHardwareConfig();
    onSetHardwareConfig();

    onGetNetworkConfig();
    onSetNetworkConfig();

    onGetServerConfig();
    onSetServerConfig();
}

ConfigurationRouter::~ConfigurationRouter()
{
}

void ConfigurationRouter::onGetHardwareConfig()
{
    server->on("/getHardwareConfig", HTTP_GET, [&](AsyncWebServerRequest *request)
               { configController->getHardwareConfig(request); });
}

void ConfigurationRouter::onSetHardwareConfig()
{
    server->on("/setHardwareConfig", HTTP_PUT, [&](AsyncWebServerRequest *request)
               { configController->setHardwareConfig(request); });
}

void ConfigurationRouter::onGetNetworkConfig()
{
    server->on("/getNetworkConfig", HTTP_GET, [&](AsyncWebServerRequest *request)
               { configController->getNetworkConfig(request); });
}

void ConfigurationRouter::onSetNetworkConfig()
{
    server->on("/setNetworkConfig", HTTP_PUT, [&](AsyncWebServerRequest *request)
               { configController->setNetworkConfig(request); });
}

void ConfigurationRouter::onGetServerConfig()
{
    server->on("/getServerConfig", HTTP_GET, [&](AsyncWebServerRequest *request)
               { configController->getServerConfig(request); });
}

void ConfigurationRouter::onSetServerConfig()
{
    server->on("/setServerConfig", HTTP_PUT, [&](AsyncWebServerRequest *request)
               { configController->setServerConfig(request); });
}
