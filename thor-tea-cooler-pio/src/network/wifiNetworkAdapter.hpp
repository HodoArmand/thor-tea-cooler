#pragma once

#include <WiFi.h>
#include "networkConfiguration.hpp"

class WifiNetworkAdapter
{
private:
    String ssid;
    String password;
    IPAddress ip;
    unsigned long checkForWifiIntervalInMs = 10000;

    int reconnectDelayInMs = 1000;
    int reconnectTimeoutCycles = 0;

public:
    WifiNetworkAdapter(NetworkConfiguration *config);
    ~WifiNetworkAdapter();

    void setSleep(bool shouldSleep);

    String getSsid() const { return ssid; }
    void setSsid(const String &ssid_) { ssid = ssid_; }

    String getPassword() const { return password; }
    void setPassword(const String &password_) { password = password_; }

    IPAddress getIp() const { return ip; }
    void setIp(const IPAddress &ip_) { ip = ip_; }

    unsigned long getCheckForWifiIntervalInMs() const { return checkForWifiIntervalInMs; }
    void setCheckForWifiIntervalInMs(unsigned long checkForWifiIntervalInMs_) { checkForWifiIntervalInMs = checkForWifiIntervalInMs_; }

    int getReconnectDelayInMs() const { return reconnectDelayInMs; }
    void setReconnectDelayInMs(int reconnectDelayInMs_) { reconnectDelayInMs = reconnectDelayInMs_; }

    int getReconnectTimeoutCycles() const { return reconnectTimeoutCycles; }
    void setReconnectTimeoutCycles(int reconnectTimeoutCycles_) { reconnectTimeoutCycles = reconnectTimeoutCycles_; }

    String ipToString(const IPAddress &ipAddress);

    bool connect();

    bool tryUntilConnected();

    bool tryUntilReConnected();
};

WifiNetworkAdapter::WifiNetworkAdapter(NetworkConfiguration *config)
{
    ssid = config->getSsid();
    password = config->getPassword();
    this->setSleep(false);
}

WifiNetworkAdapter::~WifiNetworkAdapter()
{
}

void setSleep(bool shouldSleep)
{
    WiFi.setSleep(shouldSleep);
}

String WifiNetworkAdapter::ipToString(const IPAddress &ipAddress)
{
    return String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") + String(ipAddress[2]) + String(".") + String(ipAddress[3]);
}

bool WifiNetworkAdapter::connect()
{
    wl_status_t state = WiFi.begin(ssid.c_str(), password.c_str());

    if (!state == WL_CONNECTED)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool WifiNetworkAdapter::tryUntilConnected()
{
    if (Serial && false)
    {
        Serial.println("Connecting to " + ssid + " wifi network...");
    }

    unsigned int timeOutCounter = 0;
    unsigned long currentMillis = millis();
    unsigned long previousMillis = 0;

    connect();
    while (WiFi.status() != WL_CONNECTED && timeOutCounter <= reconnectTimeoutCycles)
    {
        currentMillis = millis();
        if (currentMillis - previousMillis >= reconnectDelayInMs)
        {
            if (Serial && false)
            {
                Serial.print("...");
            }
            if (!connect() && reconnectTimeoutCycles > 0)
            {
                timeOutCounter++;
            }
            previousMillis = currentMillis;
        }
    }
    if (Serial && WiFi.status() == WL_CONNECTED)
    {
        ip = WiFi.localIP();
        Serial.println("0, OK WIFI INIT, Wifi connected, IP: " + ipToString(ip));
        return true;
    }
    else if (WiFi.status() != WL_CONNECTED && timeOutCounter == reconnectTimeoutCycles)
    {
        Serial.println("1, ERR WIFI INIT, Wifi conenction error: Timeout.");
        return false;
    }
    else
    {
        return false;
    }
}

bool WifiNetworkAdapter::tryUntilReConnected()
{
    if (Serial && false)
    {
        Serial.println("Reconnecting to " + ssid + " wifi network...");
    }

    unsigned int timeOutCounter = 0;
    unsigned long currentMillis = millis();
    unsigned long previousMillis = 0;

    WiFi.reconnect();
    while (WiFi.status() != WL_CONNECTED && timeOutCounter <= reconnectTimeoutCycles)
    {
        currentMillis = millis();
        if (currentMillis - previousMillis >= reconnectDelayInMs)
        {
            if (Serial && false)
            {
                Serial.print("...");
            }
            WiFi.reconnect();
            if (WiFi.status() != WL_CONNECTED && reconnectTimeoutCycles > 0)
            {
                timeOutCounter++;
            }
            previousMillis = currentMillis;
        }
    }
    if (Serial && WiFi.status() == WL_CONNECTED)
    {
        ip = WiFi.localIP();
        Serial.println("0, OK WIFI INIT, Wifi reconnected, IP: " + ipToString(ip));
        return true;
    }
    else if (WiFi.status() != WL_CONNECTED && timeOutCounter == reconnectTimeoutCycles)
    {
        Serial.println("1, ERR WIFI INIT, Wifi reconenction error: Timeout.");
        return false;
    }
    else
    {
        return false;
    }
}
