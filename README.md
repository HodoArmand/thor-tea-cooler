# Thor Tea Cooler - Introduction

Thor Tea Cooler is an ESP32-based IoT device specifically crafted to cool beverages and other liquids to a delightful target temperature of your choice.
This repository encompasses all the essential components: hardware, software, and comprehensive API documentation required for constructing and deploying your very own TTC (Thor Tea Cooler) device.

# Key Features & Functionalities
## Cooling & Auto thermostat

The TTC device's core function is to efficiently cool beverages using integrated cooling fans, bringing them to the desired temperature. This can be accomplished in both automatic and manual modes.

- **Automatic Mode:** In this mode, simply initiate the cooling process, and the fans will continue cooling the liquid until it reaches or goes below the target temperature (default is 50°C). Once the target temperature is achieved, the automatic cooling process halts, and the TTC device transitions to the "autoCoolingFinished" state. You have the flexibility to stop the cooling process at any time, even before reaching the target temperature.

- **Manual Mode:** In manual mode, you have direct control over the two cooling fans. You can turn them on or off at your convenience.

## Authorization

Access to the TTC device is secured using a straightforward Username/Password - Bearer token system. 
All functionalities are accessible only after a successful login. The token-based authentication system enhances the versatility of developing frontend applications for the TTC device.
Whether you're building conventional web apps, single-page applications (SPAs), or mobile apps, they can effortlessly use and store authentication tokens following a successful login.

## HTTP API

The device's functions can be interacted with trough WiFi/HTTP. Network, server and hardware settings can be configured for different hardware and software specification. For a full overview of all available API commands and HTTP Request rules, see: [API Documentation](https://github.com/HodoArmand/thor-tea-cooler/tree/dev/docs/apidoc)

![API doc sample](docs/apidoc/apiDoc%20sample.JPG)

## API Frontend Application

For a simple frontend application implementation, see: [Thor Tea Cooler | Frontend | basic HTML + CSS](https://github.com/HodoArmand/thor-tea-cooler-frontend-basic)

![Screenshot - Light mode](https://github.com/HodoArmand/thor-tea-cooler-frontend-basic/blob/main/docs/ttcScreenshotLight.JPG)

# Hardware requirements

## Parts list

- 1x ESP32 microcontroller with at least 4MB flash memory
- 1x DC12V powersource
- 1x DC12V->DV3.3V converter
- (optional) barreljacks for quick PSU/fan switching and replacement
- 1x 4.7k Ohm resistor
- 1x DS18B20 digital temperature sensor
- 2x PNP transistors
- 2x 1N4001 diodes
- 2x 3.3V switchable DC relays
- 2x 12V fans (stronger generic PC fans ideal)

## Wiring Diagram

Drawn with [KiCad](https://www.kicad.org/), a Cross Platform and Open Source Electronics Design Automation Suite project files, see: [KiCad Files](docs/hardware/thor-tea-cooler/wiring%20diagram%20-%20kiCad)

![PDF and Image](docs/hardware/thor-tea-cooler/wiring%20diagram%20-%20PDF%20and%20img/wiring_diagram.png)

# Software Build & Deploy

1. Copy the contents of the `/data_example` directory to the `/data` directory. Customize them to match your specific settings. These configurations include default user information, server settings, hardware configurations, and network settings. Initial API keys are provided but are optional, as they will be generated after the first successful login with an account.

2. (Optional) Adjust the size of the SPIFFS Filesystem if needed. This project defaults to a 2MB FLASH partition table size.

3. Build & Upload the Filesystem Image.

4. Open the .pio project file using [PlatformIO](https://platformio.org/). Connect your ESP32 device via USB/COM and click the "Upload & Monitor" option.

5. During the boot sequence, the Serial port will display each boot operation and any possible errors. For physical boot feedback, an onboard LED is used: it turns ON at the start of the boot sequence, blinks once upon successful boot, and then remains off. If an error occurs, the LED will blink three times before staying off.

# Software Architecture Overview

The project is developed using PlatformIO's Arduino-style Framework in C++11 standard, featuring a software structure with a clear separation of concerns, akin to the Model-View-Controller (MVC) pattern.

## Model Layer - Hardware Functions

Hardware functions are encapsulated within device driver classes (e.g., Relay.hpp, TemperatureSensor.hpp). These classes are organized and initialized collectively within the TtcHardware main class, which offers an internal API for interacting with the ESP32 and its peripheral hardware functions.

For instance, when executing a relay switch command in the HardwareController following successful validation of a SwitchRelayRequest, it looks like this:

```hw->switchRelay(request.getBodyParamValueByName("relay").toInt());```

This translates to an internal class structure call:

```hw->switchRelay(1); // TtcHardware::hw->Relay::relay1->toggle();```

Auth functions are structured similarly.

## Controller and Routing Layer

Similar to many web frameworks, the Controllers mediate between the routed HTTP request and the Model layer. Their responsibilities include:

- Header validation
- Validation of HTTP body values
- Authentication checks
- Execution of model manipulations for authorized and valid requests
- Sending an HTTP success/error JSON or application response to the client

## Live Data

The hardware state is continuously synchronized with the RAM model in the Hardware State class. HTTP clients can subscribe and receive live updates through a Server-Sent Events (SSE) channel, which pushes updates every 5 seconds or instantly when the hardware state changes.
