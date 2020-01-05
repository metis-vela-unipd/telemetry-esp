#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

ESP8266WiFiMulti wifiMulti;
WiFiClient espClient;
PubSubClient client(espClient);

const char *mqtt_server = "192.168.1.2";
String hostname = "WindSensor";

// void callback(char *topic, byte *payload, int length);
void startOTA();
void monitorMQTT();
void monitorWiFi();
void startmDNS();
unsigned int getWindDirection();
void ICACHE_RAM_ATTR windSpeedInterrupt();
void sendWindInfo(unsigned long time);

unsigned int windSpeed = 0;
unsigned int windDirection = 0;

unsigned long lastMsg = 0;

void setup() {
  pinMode(A0, INPUT);
  pinMode(D5, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(D5), windSpeedInterrupt, RISING);

  Serial.begin(9600);
  Serial.setDebugOutput(true);

  WiFi.hostname(hostname);
  startOTA();

  wifiMulti.addAP("QuasimodoHomeWiFi", "K1MI9391park0ur");
  wifiMulti.addAP("GioFi", "danzapk1");

  client.setServer(mqtt_server, 1883);
}

void loop() {
  monitorWiFi();
  monitorMQTT();
  ArduinoOTA.handle();
  unsigned long now = millis();
  if (now - lastMsg > 100){
    windDirection = getWindDirection();
    sendWindInfo(now);
  }
}

boolean connectionWasAlive = true;

void startmDNS() {
    if (!MDNS.begin(hostname.c_str())) {             // Start the mDNS responder
        Serial.println("Error setting up MDNS responder!");
    }
    Serial.println("mDNS responder started");
}

void startOTA() {
    ArduinoOTA.setHostname(hostname.c_str());
    ArduinoOTA.setPassword("esp8266");

    ArduinoOTA.onStart([]() {
        Serial.println("Start");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
    Serial.println("OTA ready");
}

void monitorWiFi() {
  if (wifiMulti.run() != WL_CONNECTED)
  {
    if (connectionWasAlive == true)
    {
      connectionWasAlive = false;
      Serial.print("Looking for WiFi ");
    }
    Serial.print(".");
    delay(500);
  }
  else if (connectionWasAlive == false)
  {
    connectionWasAlive = true;
    Serial.printf(" connected to %s\n", WiFi.SSID().c_str());
    startmDNS();
  }
}

void monitorMQTT() {
  if (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
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

unsigned int getWindDirection() {
  unsigned int dir;
  dir = analogRead(A0);
  return dir;
}

#define MSG_BUFFER_SIZE 50
char msg[MSG_BUFFER_SIZE];
void sendWindInfo(unsigned long time) {
  snprintf (msg, MSG_BUFFER_SIZE, "%1d, %1d, %1ld", windSpeed, windDirection, time);
  client.publish("WindInfo", msg);
}

unsigned long lastSpd = 0;
void ICACHE_RAM_ATTR windSpeedInterrupt() {
  unsigned long spd = millis();
  windSpeed = spd - lastSpd;
  lastSpd = spd;
}
