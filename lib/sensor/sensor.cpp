#include <sensor.h>

// Sensor constructor
sensor::sensor(PubSubClient client)
{
    this->client = client;
}

// General setup method
void sensor::setup()
{
}

// Sensor loop, the action method will be called every SENSOR_UPDATE_INTERVAL ms
void sensor::loop()
{
    unsigned long now = millis();
    if (now - this->lastMsgTime > SENSOR_UPDATE_INTERVAL)
    {
        this->action();
        this->sendData(now);
        this->lastMsgTime = now;
    }
}