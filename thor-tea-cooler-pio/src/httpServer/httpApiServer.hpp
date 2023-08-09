#pragma once

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Authorization.hpp"

#include "routing/AuthRouter.hpp"
#include "routing/HardwareRouter.hpp"
#include "routing/ConfigurationRouter.hpp"

enum ServerState
{
    SRV_UNINITIALIZED,
    AUTH_LOAD_ERROR,
    AUTH_LOADED,
    SRV_INITIALIZED,
    SRV_RUNNING,
    SRV_STOPPED
};

class HttpApiServer
{
private:
    ServerState state = SRV_UNINITIALIZED;

public:
    HttpApiServer(ServerConfiguration *config_, TtcHardware *hw_, HardwareConfiguration *hwConfig_, NetworkConfiguration *networkConfig_, ServerConfiguration *serverConfig_);
    ~HttpApiServer();

    TtcHardware *hw;
    Authorization *auth;
    AsyncWebServer *server;

    HardwareConfiguration *hwConfig;
    NetworkConfiguration *networkConfig;
    ServerConfiguration *serverConfig;

    ServerState getState() const { return state; }
    void setState(const ServerState &state_) { state = state_; }

    //  Route

    AuthRouter *authRouter;
    HardwareRouter *hwRouter;
    ConfigurationRouter *configRouter;

    void initializeApi();
    void startApi();

    void initializeWebSockets();
    void startWebSockets();
};

HttpApiServer::HttpApiServer(ServerConfiguration *config_, TtcHardware *hw_, HardwareConfiguration *hwConfig_, NetworkConfiguration *networkConfig_, ServerConfiguration *serverConfig_)
{
    auth = new Authorization(config_);
    hw = hw_;

    hwConfig = hwConfig_;
    networkConfig = networkConfig_;
    serverConfig = serverConfig_;

    if (auth->initFileSystem() && auth->loadUsersFromDisk() && auth->loadApiKeysFromDisk())
    {
        setState(AUTH_LOADED);
        server = new AsyncWebServer(config_->getPort());
    }
    else
    {
        setState(AUTH_LOAD_ERROR);
    }
}

HttpApiServer::~HttpApiServer()
{
}

void HttpApiServer::initializeApi()
{
    server->onNotFound([](AsyncWebServerRequest *request)
                       { Controller::simpleNotFoundResponse(request); });

    authRouter = new AuthRouter(server, auth);
    hwRouter = new HardwareRouter(server, hw, auth);
    configRouter = new ConfigurationRouter(server, auth, hwConfig, networkConfig, serverConfig);

    setState(SRV_INITIALIZED);
}

void HttpApiServer::startApi()
{
    server->begin();
    setState(SRV_RUNNING);
    Serial.println("Server initialized, running...");
}

inline void HttpApiServer::initializeWebSockets()
{
    Serial.println("WS:TODO");
}

inline void HttpApiServer::startWebSockets()
{
    Serial.println("WS:TODO");
}
