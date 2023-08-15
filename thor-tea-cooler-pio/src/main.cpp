#include "main.hpp"

HardwareConfiguration *hwConfig;
TtcHardware *hw;
NetworkConfiguration *networkConfig;
WifiNetworkAdapter *wifiAdapter;
ServerConfiguration *serverConfig;
HttpApiServer *server;

//------------------------------------------------   Main   ---------------------------------

void setup()
{
    initializeSerial(115200, 1500);
    loadHardwareConfig();
    initializeHardware();
    loadNetworkConfig();
    connectToWifi();
    loadServerConfig();
    initializeApiServer();
    server->startApi();
    //  loadOscClientConfig();
    //  initializeOscClient();
}

void loop()
{
    delay(5000); // TODO: replace this with timer
    hw->readHardwareState();
    if (hw->getMode() == autoCooling && hw->getTemperature() <= hw->getTargetTemperature())
    {
        hw->stopCooling();
    }

    server->broadcastTeaState();

    if (hwConfig->getDebugMode())
    {
        Serial.println(hw->getHardwareStateAsJsonString());
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        wifiAdapter->tryUntilReConnected();
    }
}

//------------------------------------------------   BOOT functions   ---------------------------------

void initializeSerial(int baudRate, int delayMs)
{
    Serial.begin(baudRate);
    delay(delayMs); //  let Serial boot up for proper boot sequence print
    Serial.println();
    Serial.println("----------------------------------------");
    Serial.println("-------   TTC v1.0 booting...   --------");
    Serial.println("----------------------------------------");
}

void loadHardwareConfig()
{
    Serial.println("BOOT: Loading hardware configuration...");

    hwConfig = new HardwareConfiguration();

    hwConfig->setConfigFileName("/hardwareConfig.json");
    hwConfig->setConfigFileMaxSize(2048);

    bool isFs = hwConfig->initFileSystem();

    if (!isFs || !hwConfig->loadFromDisk())
    {
        Serial.println("1, ERR - Hardware configuration load failed, restarting in 10 seconds...");
        delay(10000);
        ESP.restart();
    }
}

void initializeHardware()
{

    Serial.println("BOOT: Initializing hardware...");

    hw = new TtcHardware(hwConfig);

    if (hw->relay1->getState() != INITIALIZED || hw->relay2->getState() != INITIALIZED)
    {
        Serial.println("1, ERR - Hardware initialization failed, Relay initialization error. Restarting in 5 seconds...");
        delay(5000);
        ESP.restart();
    }
    else
    {
        Serial.println("0, OK - Relays initialized.");
        hw->relay1->off();
        hw->relay2->off();

        Serial.println("0, OK - Temperature sensor initialized.");

        Serial.println("0, OK - Hardware initialization successful.");
    }
}

void loadNetworkConfig()
{
    Serial.println("BOOT: Loading network configuration...");

    networkConfig = new NetworkConfiguration();

    networkConfig->setConfigFileName("/networkConfig.json");
    networkConfig->setConfigFileMaxSize(256);

    bool isFs = networkConfig->initFileSystem();

    if (!isFs || !networkConfig->loadFromDisk())
    {
        Serial.println("1, ERR - Network configuration load failed, restarting in 10 seconds...");
        delay(10000);
        ESP.restart();
    }
}

void connectToWifi()
{
    Serial.println("BOOT: Connecting to wifi network...");

    wifiAdapter = new WifiNetworkAdapter(networkConfig);

    if (!wifiAdapter->tryUntilConnected())
    {
        Serial.println("1, ERR - Wifi connection failed, restarting in 10 seconds...");
        delay(10000);
        ESP.restart();
    }
}

void loadServerConfig()
{
    serverConfig = new ServerConfiguration();

    serverConfig->setConfigFileName("/serverConfig.json");
    serverConfig->setConfigFileMaxSize(1024);

    bool isFs = serverConfig->initFileSystem();

    if (!isFs || !serverConfig->loadFromDisk())
    {
        Serial.println("1, ERR - Server configuration load failed, restarting in 10 seconds...");
        delay(10000);
        ESP.restart();
    }
}

void initializeApiServer()
{
    server = new HttpApiServer(serverConfig, hw, hwConfig, networkConfig, serverConfig);
    server->initializeApi();
}