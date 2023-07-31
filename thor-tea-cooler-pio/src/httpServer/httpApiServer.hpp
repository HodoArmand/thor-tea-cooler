#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "AuthController.hpp"

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
    HttpApiServer(ServerConfiguration *config_);
    ~HttpApiServer();

    AuthController *auth;
    AsyncWebServer *server;

    ServerState getState() const { return state; }
    void setState(const ServerState &state_) { state = state_; }

    void initializeApi();
    void start();
};

HttpApiServer::HttpApiServer(ServerConfiguration *config_)
{
    auth = new AuthController(config_);

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
    setState(SRV_INITIALIZED);
}

void HttpApiServer::start()
{
    server->begin();
    setState(SRV_RUNNING);
}
