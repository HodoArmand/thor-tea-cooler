#pragma once

#include <Arduino.h>
#include <PsychicHttp.h>

class Router
{
private:
    PsychicHttpsServer *server;

public:
    Router();
    Router(PsychicHttpsServer *server_);
    ~Router();
};

inline Router::Router()
{
}

Router::Router(PsychicHttpsServer *server_)
{
    server = server_;
}

Router::~Router()
{
}
