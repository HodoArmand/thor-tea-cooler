#pragma once

// #include <AsyncTCP.h>
#include "../lib/AsyncTCPQuemod/AsyncTCP.h"
// // #include <ESPPsychicHttpServer.h>
#include <PsychicHttp.h>
#include <PsychicHttp.h>

#include "Authorization.hpp"

#include "routing/AuthRouter.hpp"
#include "routing/HardwareRouter.hpp"
#include "routing/ConfigurationRouter.hpp"
#include "routing/ServerSideEventRouter.hpp"

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
    PsychicHttpServer *server;

    HardwareConfiguration *hwConfig;
    NetworkConfiguration *networkConfig;
    ServerConfiguration *serverConfig;

    ServerState getState() const { return state; }
    void setState(const ServerState &state_) { state = state_; }

    void broadcastTeaState();

    //  Route

    AuthRouter *authRouter;
    HardwareRouter *hwRouter;
    ConfigurationRouter *configRouter;
    ServerSideEventRouter *sseRouter;

    void initializeApi();
    void startApi();
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
        server = new PsychicHttpServer();
    }
    else
    {
        setState(AUTH_LOAD_ERROR);
    }
}

HttpApiServer::~HttpApiServer()
{
}

inline void HttpApiServer::broadcastTeaState()
{
    sseRouter->sendTeaState();
}

void HttpApiServer::initializeApi()
{
    server->config.max_uri_handlers = 100;
    server->listen(80);
    server->onNotFound([](PsychicRequest *request)
                       {
        if (request->method() == HTTP_OPTIONS)
        {
            return request->reply(200);
        }
        else
        {
            return Controller::simpleNotFoundResponse(request);
        } });

    server->on("/isTtc", HTTP_GET, [&](PsychicRequest *request)
               { return Controller::simpleResponse(request, 200, "yes", "Yes, TTC Device."); });

    sseRouter = new ServerSideEventRouter(hw, server, "/events");

    authRouter = new AuthRouter(server, auth);
    hwRouter = new HardwareRouter(server, hw, auth, sseRouter);
    configRouter = new ConfigurationRouter(server, auth, hwConfig, networkConfig, serverConfig);

    if (serverConfig->getDebugMode())
    {
        DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
        DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
        DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");
        DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Authorization");
    }

    setState(SRV_INITIALIZED);
}

void HttpApiServer::startApi()
{
    setState(SRV_RUNNING);
    Serial.println("Server initialized, running...");
}
