#pragma once

#include <Arduino.h>
#include <vector>

struct Timer
{
    String name;
    unsigned long intervalInMillis;
    unsigned long lastDoneAtMillis;
};

class Timing
{
private:
    unsigned long now = 0;
    std::vector<Timer> timers;

public:
    Timing(/* args */);
    ~Timing();

    unsigned long getNow();
    void refreshNow();
    void setNow(unsigned long ms);

    void addTimer(String name_, unsigned long intervalInMillis_);
    void removeTimer(String name_);

    bool isTimeFor(String timerName);
    void setLastDoneNowFor(String timerName);

    void printToSerial();
};

Timing::Timing(/* args */)
{
}

Timing::~Timing()
{
}

unsigned long Timing::getNow()
{
    return this->now;
}

void Timing::refreshNow()
{
    this->now = millis();
}

void Timing::setNow(unsigned long ms)
{
    this->now = ms;
}

void Timing::addTimer(String name_, unsigned long intervalInMillis_)
{
    Timer newTimer = {.name = name_, .intervalInMillis = intervalInMillis_, .lastDoneAtMillis = 0};
    timers.reserve(timers.size() + 1);
    timers.push_back(newTimer);
}

void Timing::removeTimer(String name_)
{
    for (int i = 0; i < timers.size(); i++)
    {
        if (timers[i].name == name_)
        {
            timers.erase(timers.begin() + i);
            timers.shrink_to_fit();
            break;
        }
    }
}

bool Timing::isTimeFor(String timerName)
{
    for (Timer t : timers)
    {
        if (t.name == timerName)
        {
            return (now >= t.lastDoneAtMillis + t.intervalInMillis);
        }
    }

    return false;
}

void Timing::setLastDoneNowFor(String timerName)
{
    for (Timer &t : timers)
    {
        if (t.name == timerName)
        {
            t.lastDoneAtMillis = this->getNow();
            break;
        }
    }
}

void Timing::printToSerial()
{
    Serial.println("Timing");
    Serial.println("-Now: " + (String)this->getNow());
    for (Timer t : timers)
    {
        Serial.println("Name: " + t.name + " Interval: " + t.intervalInMillis + " Last done @: " + t.lastDoneAtMillis);
    }
}