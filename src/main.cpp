#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Esp.h>

#define NB_TRYWIFI      10
#define SLEEP_DURATION  60 * 1e6

#define SSID "MètisDataNet"
#define PSW  "metis2020"

WiFiClient espClient;
PubSubClient client(espClient);

#ifdef SENSOR_WIND
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

const char *mqtt_server = "192.168.4.1";

// void callback(char *topic, byte *payload, int length);
void monitorMQTT();
void monitorWiFi();
void startmDNS();

void setup() {
  Serial.begin(9600);
  //Serial.setDebugOutput(true);

  WiFi.hostname(HOSTNAME);

  WiFi.begin(SSID, PSW);

  Serial.print("Connecting");
  int _try = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    _try++;
    Serial.print(".");
    if ( _try >= NB_TRYWIFI ) {
        Serial.println("Impossible to connect WiFi network, going to deep sleep, goodnight!");
        ESP.deepSleep(SLEEP_DURATION);
    }
  }
  Serial.println();

  startmDNS();

  client.setServer(mqtt_server, 1883);

  sens.setup();
}

void loop() {
  monitorMQTT();
  
  sens.loop();

  if (WiFi.status() != WL_CONNECTED){
    Serial.println("Lost Connection to WiFi, going to deep sleep, goodnight!");
    ESP.deepSleep(SLEEP_DURATION);
  }
}

boolean connectionWasAlive = true;

void startmDNS() {
    if (!MDNS.begin(HOSTNAME)) {             // Start the mDNS responder
        Serial.println("Error setting up MDNS responder!");
    }
    Serial.println("mDNS responder started");
}

void monitorMQTT() {
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
  } else {
     client.loop();
  }
}

