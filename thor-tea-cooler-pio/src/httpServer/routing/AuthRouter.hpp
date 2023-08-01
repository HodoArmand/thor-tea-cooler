#pragma once

#include "Router.hpp"
#include "httpServer/Controllers/AuthController.hpp"

class AuthRouter : public Router
{

private:
    AsyncWebServer *server;
    AuthController *authController;

    void onLogin();
    void onLogout();
    void onRegister();
    void onEdit();
    void onDelete();

public:
    AuthRouter(AsyncWebServer *server_, Authorization *auth);
    ~AuthRouter();
};

inline AuthRouter::AuthRouter(AsyncWebServer *server_, Authorization *auth)
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
    server->on("/login", HTTP_POST, [&](AsyncWebServerRequest *request)
               { authController->login(request); });
}

void AuthRouter::onLogout()
{
    server->on("/logout", HTTP_POST, [&](AsyncWebServerRequest *request)
               { authController->logout(request); });
}
void AuthRouter::onRegister()
{
    server->on("/registerUser", HTTP_POST, [&](AsyncWebServerRequest *request)
               { authController->registerUser(request); });
}
void AuthRouter::onEdit()
{
    server->on("/editUser", HTTP_PUT, [&](AsyncWebServerRequest *request)
               { authController->editUser(request); });
}
void AuthRouter::onDelete()
{
    server->on("/deleteUser", HTTP_DELETE, [&](AsyncWebServerRequest *request)
               { authController->deleteUser(request); });
}
