#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Esp.h>

#ifdef DEBUG
    #include <GDBStub.h>
#endif

#define NB_TRYWIFI 10           // Number of wifi connection tries before going to sleep
#define SLEEP_DURATION 60 * 1e6 // Sleep duration [us]

#define SSID "MètisDataNet" // WiFi credentials
#define PSW "metis2020"


#define MQTT_SERVER_IP "192.168.1.2"

WiFiClient espClient;
PubSubClient client(espClient);

#ifdef SENSOR_WIND // Sensor selection defines, selected in environment
    #include <windSensor.h>
    windSensor sens(client);
#endif
#ifdef SENSOR_ACCEL
    #include <accelSensor.h>
    accelSensor sens(client);
#endif
#ifdef SENSOR_STRAIN
    #include <strainSensor.h>
    strainSensor sens(client);
#endif

#ifndef HOSTNAME
    #define HOSTNAME "SensorUnknown"
#endif

// void callback(char *topic, byte *payload, int length);
void monitorMQTT();
void monitorWiFi();
void startmDNS();

void setup()
{
    Serial.begin(115200);
    #ifdef DEBUG
        Serial.setDebugOutput(true);
        gdbstub_init();
    #endif

    //WiFi setup, with hostnale
    WiFi.hostname(HOSTNAME);
    WiFi.begin(SSID, PSW);

    // Try to connect, if unable go to sleep
    Serial.print("Connecting");
    int _try = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        _try++;
        Serial.print(".");
        if (_try >= NB_TRYWIFI)
        {
            Serial.println("Impossible to connect WiFi network, going to deep sleep, goodnight!");
            ESP.deepSleep(SLEEP_DURATION);
        }
    }
    Serial.println();

    // DNS setup
    startmDNS();

    // mQTT server setup
    client.setServer(MQTT_SERVER_IP, 1883);

    // Sensor setup
    sens.setup();
}

void loop()
{
    // mQtt and sensor loops
    monitorMQTT();
    sens.loop();

    // If we are not connected to WiFi anymore go to sleep
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Lost Connection to WiFi, going to deep sleep, goodnight!");
        ESP.deepSleep(SLEEP_DURATION);
    }
}

void startmDNS()
{
    if (!MDNS.begin(HOSTNAME))
    { // Start the mDNS responder
        Serial.println("Error setting up MDNS responder!");
    }
    Serial.println("mDNS responder started");
}

void monitorMQTT()
{
    if (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(HOSTNAME))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
    else
    {
        client.loop();
    }
}
