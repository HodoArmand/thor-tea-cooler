#pragma once

#include "Router.hpp"
#include "httpServer/Controllers/AuthController.hpp"

class AuthRouter : public Router
{

private:
    PsychicHttpServer *server;
    AuthController *authController;

    void onLogin();
    void onLogout();
    void onRegister();
    void onEdit();
    void onDelete();

public:
    AuthRouter(PsychicHttpServer *server_, Authorization *auth);
    ~AuthRouter();
};

inline AuthRouter::AuthRouter(PsychicHttpServer *server_, Authorization *auth)
{
    server = server_;
    authController = new AuthController(auth);

    onLogin();
    onLogout();
    onRegister();
    onEdit();
    onDelete();
}

AuthRouter::~AuthRouter()
{
}

void AuthRouter::onLogin()
{
    server->on("/login", HTTP_POST, [&](PsychicRequest *request, JsonVariant &json)
               { return authController->login(request, json); });
}

void AuthRouter::onLogout()
{
    server->on("/logout", HTTP_POST, [&](PsychicRequest *request)
               { return authController->logout(request); });
}
void AuthRouter::onRegister()
{
    server->on("/registerUser", HTTP_POST, [&](PsychicRequest *request, JsonVariant &json)
               { return authController->registerUser(request, json); });
}
void AuthRouter::onEdit()
{
    server->on("/editUser", HTTP_PUT, [&](PsychicRequest *request, JsonVariant &json)
               { return authController->editUser(request, json); });
}
void AuthRouter::onDelete()
{
    server->on("/deleteUser", HTTP_DELETE, [&](PsychicRequest *request, JsonVariant &json)
               { return authController->deleteUser(request, json); });
}
