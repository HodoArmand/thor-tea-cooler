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

#include "ds18B20-universal-driver.hpp"

void DsDriver::vectorAddressToDeviceAddress(std::vector<uint8_t> vectAddress, DeviceAddress &address)
{
    for (size_t i = 0; i < 8; i++)
    {
        address[i] = vectAddress.at(i);
    }
}

void DsDriver::deviceAddressToVectorAddress(std::vector<uint8_t> &vectAddress, DeviceAddress address)
{
    for (size_t i = 0; i < 8; i++)
    {
        vectAddress.push_back(address[i]);
    }
}

String DsDriver::deviceAddressToString(DeviceAddress address)
{
    String addressString = "";
    try
    {
        for (size_t i = 0; i < 8; i++)
        {
            if (i != 7)
            {
                addressString += (String(address[i], DEC) + " ");
            }
            else
            {
                addressString += (String(address[i], DEC));
            }
        }
    }
    catch (const std::exception &e)
    {
        // std::cerr << e.what() << '\n';
        // Serial.println(e.what());
        addressString = "00 00 00 00 00 00 00 00";
    }
    return addressString;
}

String DsDriver::vectorAddressToString(std::vector<uint8_t> address)
{
    String addressString = "";
    try
    {
        for (size_t i = 0; i < 8; i++)
        {
            if (i != 7)
            {
                addressString += (String(address.at(i), DEC) + " ");
            }
            else
            {
                addressString += (String(address.at(i), DEC));
            }
        }
    }
    catch (const std::exception &e)
    {
        // std::cerr << e.what() << '\n';
        addressString = "00 00 00 00 00 00 00 00";
    }
    return addressString;
}

void DsDriver::stringToVectorAddress(String addressString, std::vector<uint8_t> deviceAddress)
{
    size_t outPutIterator = 0;
    deviceAddress.reserve(8);

    while (addressString.length() > 0 && outPutIterator < 8)
    {
        int indexOfSpaceChar = addressString.indexOf(' ');
        if (indexOfSpaceChar == -1) // delim not found
        {
            deviceAddress[outPutIterator++] = addressString.toInt();
            break;
        }
        else
        {
            deviceAddress[outPutIterator++] = (addressString.substring(0, indexOfSpaceChar)).toInt();
            addressString = addressString.substring(indexOfSpaceChar + 1);
        }
    }
    deviceAddress.shrink_to_fit();
}

//  TODO: add HED string output version
void DsDriver::stringToDeviceAddress(String addressString, DeviceAddress &deviceAddress)
{
    size_t outPutIterator = 0;

    while (addressString.length() > 0 && outPutIterator < 8)
    {
        int indexOfSpaceChar = addressString.indexOf(' ');
        if (indexOfSpaceChar == -1) // delim not found
        {
            deviceAddress[outPutIterator++] = addressString.toInt();
            break;
        }
        else
        {
            deviceAddress[outPutIterator++] = (addressString.substring(0, indexOfSpaceChar)).toInt();
            addressString = addressString.substring(indexOfSpaceChar + 1);
        }
    }
}

void DsDriver::scanSensors()
{
    bool isSensorFound = true;
    DeviceAddress buffer;
    int iterator = 0;

    while (isSensorFound)
    {
        isSensorFound = this->temperatureSensors.getAddress(buffer, iterator);
        if (isSensorFound)
        {
            if (Serial && this->isDebug)
            {
                Serial.println("Sensor found, address:");
                Serial.println(buffer[0], HEX);
                Serial.println(buffer[1], HEX);
                Serial.println(buffer[2], HEX);
                Serial.println(buffer[3], HEX);
                Serial.println(buffer[4], HEX);
                Serial.println(buffer[5], HEX);
                Serial.println(buffer[6], HEX);
                Serial.println(buffer[7], HEX);
            }

            std::vector<uint8_t> buffervector;
            deviceAddressToVectorAddress(buffervector, buffer);
            this->temperatureSensorAddressArray.push_back(buffervector);
            iterator++;
        }
        else
        {
            break;
        }
    }

    if (this->isDebug && Serial)
    {
        Serial.println("# of Devices found: " + String(this->temperatureSensorAddressArray.size()));
        if (this->temperatureSensorAddressArray.size() > 0)
        {
            String line;
            for (int i = 0; i < this->temperatureSensorAddressArray.size(); i++)
            {
                line = "Sensor # " + String(i) + " address: " + vectorAddressToString(this->temperatureSensorAddressArray.at(i));
                Serial.println(line);
            }
        }
    }
}

void DsDriver::listSensorsToSerial()
{
    if (Serial && this->isDebug)
    {
        Serial.println("Listing sensors...");
        Serial.println("# of Devices stored: " + String(this->temperatureSensorAddressArray.size()));
        if (this->temperatureSensorAddressArray.size() > 0)
        {
            String line;
            for (int i = 0; i < this->temperatureSensorAddressArray.size(); i++)
            {
                line = "Sensor #" + String(i) + "address: " + vectorAddressToString(this->temperatureSensorAddressArray.at(i));
                Serial.println(line);
            }
        }
        else
        {
            Serial.println("No sensors to list.");
        }
    }
}

void DsDriver::readSensortemperaturesInCelsius()
{
    if (Serial && isDebug)
    {
        Serial.println("Reading sensors...");
    }
    if (temperatureSensorAddressArray.size() > 0)
    {
        temperatureSensors.requestTemperatures();
        float temperatureInCelsius;
        for (int i = 0; i < temperatureSensorAddressArray.size(); i++)
        {
            temperaturesArray.clear();
            temperaturesArray.reserve(8);
            temperatureInCelsius = temperatureSensors.getTempCByIndex(i);
            if (Serial && this->isDebug)
            {
                Serial.println("Measured value: "+ String(temperatureInCelsius));
            }
            if (temperatureInCelsius != -127)
            {
                temperatureInCelsius += temperatureCalibrationOffset;
            }

            temperaturesArray.push_back(temperatureInCelsius);
        }
        if (Serial && this->isDebug)
        {
            Serial.println("Sensor data stored.");
        }
    }
    else if (Serial && this->isDebug)
    {
        Serial.println("No sensors to read temperature from.");
    }
}

float DsDriver::readSensortemperatureInCelsiusByIndex(int index)
{
    this->readSensortemperaturesInCelsius();
    if (index >= this->temperaturesArray.size())
    {
        if (Serial && this->isDebug)
        {
            Serial.println("Index not found in temperatures array.");
        }

        return -999.99;
    }
    else
    {
        if (Serial && this->isDebug)
        {
            String line = "Index #" + String(index) + "'s measured temperature in C: " + String(this->temperaturesArray.at(index));
            Serial.println(line);
        }
        return this->temperaturesArray.at(index);
    }
}

void DsDriver::printTemperatures()
{
    if (Serial)
    {
        if (this->temperaturesArray.size() > 0)
        {
            String line;
            for (int i = 0; i < this->temperaturesArray.size(); i++)
            {
                line = "#" + String(i) + ": " + String(this->temperaturesArray.at(i));
                Serial.println(line);
            }
        }
        else
        {
            Serial.println("No temperature data to display.");
        }
    }
}