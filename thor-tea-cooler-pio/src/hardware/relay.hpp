#include <string>
#include <Arduino.h>

using namespace std;

enum RelayPinError
{
    ZERO,
    DEBUG_PIN,
    PIN_OK,
    RX_PIN,
    SPI_FLASH_PIN,
    INPUT_ONLY_PIN,
    NOT_ESP32_PIN
};

enum RelayState
{
    UNINITIALIZED,
    ERROR,
    INITIALIZED
};

class Relay
{
private:
    int ioPin;
    RelayState state = UNINITIALIZED;

public:
    Relay(int pinNumber);
    static RelayPinError validateIoPin(int pinNumber);
    void on();
    void off();
    void toggle();
    int read();
    bool isOn();
    RelayState getState();
    ~Relay();
};

Relay::Relay(int pinNumber)
{
    if (validateIoPin(pinNumber) != PIN_OK)
    {
        state = ERROR;
    }
    else
    {
        ioPin = pinNumber;
        pinMode(ioPin, OUTPUT);
        digitalWrite(ioPin, LOW);
        state = INITIALIZED;
    }
}

inline void Relay::on()
{
    digitalWrite(ioPin, HIGH);
}

inline void Relay::off()
{
    digitalWrite(ioPin, LOW);
}

inline void Relay::toggle()
{
    digitalWrite(ioPin, !digitalRead(ioPin));
}

inline int Relay::read()
{
    return digitalRead(ioPin);
}

inline bool Relay::isOn()
{
    return digitalRead(ioPin) == HIGH;
}

inline RelayState Relay::getState()
{
    return state;
}

Relay::~Relay()
{
}

RelayPinError Relay::validateIoPin(int pinNumber)
{
    switch (pinNumber)
    {
    case 0:
        return ZERO;
        break;

    case 1:
        return DEBUG_PIN;
        break;

    case 2:
        return PIN_OK;
        break;

    case 3:
        return RX_PIN;
        break;

    case 4 ... 5:
        return PIN_OK;
        break;

    case 6 ... 11:
        return SPI_FLASH_PIN;
        break;

    case 12 ... 33:
        return PIN_OK;
        break;

    case 34 ... 39:
        return INPUT_ONLY_PIN;
        break;

    default:
        return NOT_ESP32_PIN;
        break;
    }
}
