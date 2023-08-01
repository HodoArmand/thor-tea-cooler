#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Authorization.hpp"

#include "routing/AuthRouter.hpp"
#include "routing/HardwareRouter.hpp"

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
    HttpApiServer(ServerConfiguration *config_, TtcHardware *hw_);
    ~HttpApiServer();

    TtcHardware *hw;
    Authorization *auth;
    AsyncWebServer *server;

    ServerState getState() const { return state; }
    void setState(const ServerState &state_) { state = state_; }

    //  Route

    AuthRouter *authRouter;
    HardwareRouter *hwRouter;

    void initializeApi();
    void start();
};

HttpApiServer::HttpApiServer(ServerConfiguration *config_, TtcHardware *hw_)
{
    auth = new Authorization(config_);
    hw = hw_;

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
    authRouter = new AuthRouter(server, auth);
    hwRouter = new HardwareRouter(server, hw, auth);

    setState(SRV_INITIALIZED);
}

void HttpApiServer::start()
{
    server->begin();
    setState(SRV_RUNNING);
    Serial.println("Server initialized, running...");
}
