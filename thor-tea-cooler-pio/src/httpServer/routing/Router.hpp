#pragma once

#include <Arduino.h>
#include <PsychicHttp.h>
#include <PsychicHttp.h>

class Router
{
private:
    PsychicHttpServer *server;

public:
    Router();
    Router(PsychicHttpServer *server_);
    ~Router();
};

inline Router::Router()
{
}

Router::Router(PsychicHttpServer *server_)
{
    server = server_;
}

Router::~Router()
{
}
