#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Esp.h>

WiFiClient espClient;
PubSubClient client(espClient);

const char *mqtt_server = "192.168.4.1";
String hostname = "WindSensor";

// void callback(char *topic, byte *payload, int length);
void monitorMQTT();
void monitorWiFi();
void startmDNS();
unsigned int getWindDirection();
void ICACHE_RAM_ATTR windSpeedInterrupt();
void sendWindInfo(unsigned long time);

#define NB_TRYWIFI      10
#define SLEEP_DURATION  60 * 1e6

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

  WiFi.begin("MètisDataNet", "metis2020");

  Serial.print("Connecting");
  int _try = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    _try++;
    Serial.print(".");
    if ( _try >= NB_TRYWIFI ) {
        Serial.println("Impossible to connect WiFi network, going to deep sleep");
        ESP.deepSleep(SLEEP_DURATION);
    }
  }
  Serial.println();

  startmDNS();

  client.setServer(mqtt_server, 1883);
}

void loop() {
  monitorMQTT();
  unsigned long now = millis();

  if (now - lastMsg > 100){
    //windDirection = getWindDirection();
    sendWindInfo(now);
    lastMsg = now;
  }

  if (WiFi.status() != WL_CONNECTED){
    Serial.println("Lost Connection to WiFi, going to deep sleep");
    ESP.deepSleep(SLEEP_DURATION);
  }
}

boolean connectionWasAlive = true;

void startmDNS() {
    if (!MDNS.begin(hostname.c_str())) {             // Start the mDNS responder
        Serial.println("Error setting up MDNS responder!");
    }
    Serial.println("mDNS responder started");
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

// 1000ms == 1.29kn == 2.4km/h
unsigned long lastSpd = 0;
void ICACHE_RAM_ATTR windSpeedInterrupt() {
  unsigned long spd = millis();
  if (spd - lastSpd >= 10)
  {
    windSpeed = spd - lastSpd;
    lastSpd = spd;
  }
}

