#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

class Router
{
private:
    AsyncWebServer *server;

public:
    Router();
    Router(AsyncWebServer *server_);
    ~Router();
};

inline Router::Router()
{
}

Router::Router(AsyncWebServer *server_)
{
    server = server_;
}

Router::~Router()
{
}
