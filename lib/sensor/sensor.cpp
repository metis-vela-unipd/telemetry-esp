#include <sensor.h>

// Sensor constructor
sensor::sensor(PubSubClient client)
{
    this->client = client;
}

// General setup method
void sensor::setup()
{
    #ifdef DEBUG
        Serial.println("Sensor setup occurring!");
    #endif
    for (int i = 0; i < SENSOR_DATA_N; i++)
    {
        dataArray[i].topic = MQTT_TOPIC;
        dataArray[i].topic += getTopic();
    }
}

// Sensor loop, the action method will be called every SENSOR_UPDATE_INTERVAL ms
void sensor::loop()
{
    unsigned long now = millis();
    if (now - this->lastMsgTime > SENSOR_UPDATE_INTERVAL)
    {
        action();
        sendData(now);
        this->lastMsgTime = now;
    }
}

void sensor::sendData(unsigned long time)
{
    for (int i = 0; i < SENSOR_DATA_N; i++)
    {
        char msg[MSG_BUFFER_SIZE] = "";
        snprintf(msg, MSG_BUFFER_SIZE, "%1d, %1d", dataArray[i].value, time);
        this->client.publish(dataArray[i].topic.c_str(), msg);
        #ifdef DEBUG
            Serial.print(dataArray[i].topic);
            Serial.print(": ");
            Serial.println(msg);
        #endif
    }
}