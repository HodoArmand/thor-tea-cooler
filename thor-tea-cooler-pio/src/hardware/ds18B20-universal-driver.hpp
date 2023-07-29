//          @@
//         @@@&
//        @@@@@/
// @@@@@@@@@@@@@.
//  @@@@@@@@@@@@@@@&
//    @@@@@@@@@@@@@@@@@@
//     @@@@@@@@@@@@@@@   @@@ @@@  @
//    @@@@@@@@@@@@@@ @@@#@@@ @@@ &@@. @@@ @@
//    @@@@@@@@@@@@@ .@@% @@% @@@ ,@@  @@( @@(  &
//    @@@@@  .@@@@@,@@@@@@@@ @@@ @@@%*@@@ @@@ &@@  @@
//              @@@@ @@  @@  .@(  @@  &@   #   (.  ./
//                 @          @@ ,@@@&@@@ @@@ @@@ ,@@* @@@ %@@
//                                              @  @@# @@@ @@@  @@  @@
//            Header by H.A.  2021-2023                      @  @@  @@# *@@  @
//                                                                       @(  @@
//		Universal Dallas Semiconductor DS18B20 sigital sensor driver wrapper

#pragma once

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <vector>

class DsDriver
{
private:
    bool isDebug = false;
    float temperatureCalibrationOffset = 0.00;

public:
    std::vector<std::vector<uint8_t>> temperatureSensorAddressArray;
    std::vector<float> temperaturesArray;

    DallasTemperature temperatureSensors;

    DsDriver()
    {
        this->temperatureSensorAddressArray.clear();
    }
    DsDriver(float temperatureCalibrationOffsetInput, OneWire &oneWire, bool isDebugInput)
    {
        this->isDebug = isDebugInput;
        this->setTemperatureCalibrationOffset(temperatureCalibrationOffsetInput);
        this->temperatureSensors = DallasTemperature(&oneWire);
        this->temperatureSensors.begin();

        if (Serial && this->isDebug)
        {
            Serial.println("0, DS18B20-DRIVER - DS driver initialized.");
        }
    }

    static void vectorAddressToDeviceAddress(std::vector<uint8_t> vectAddress, DeviceAddress &address);
    static void deviceAddressToVectorAddress(std::vector<uint8_t> &vectAddress, DeviceAddress address);

    static String deviceAddressToString(DeviceAddress address);
    static String vectorAddressToString(std::vector<uint8_t> address);
    static void stringToVectorAddress(String addressString, std::vector<uint8_t> deviceAddress);
    static void stringToDeviceAddress(String addressString, DeviceAddress &deviceAddress);

    void scanSensors();

    void listSensorsToSerial();

    void readSensortemperaturesInCelsius();
    float readSensortemperatureInCelsiusByIndex(int index);

    void printTemperatures();

    static void setDeviceAddress(DeviceAddress &target, DeviceAddress input)
    {
        for (size_t i = 0; i < 8; i++)
        {
            target[i] = input[i];
        }
    }

    float getTemperatureCalibrationOffset()
    {
        return temperatureCalibrationOffset;
    }

    void setTemperatureCalibrationOffset(float temperatureCalibrationOffsetInput)
    {
        this->temperatureCalibrationOffset = temperatureCalibrationOffsetInput;
    }
};
