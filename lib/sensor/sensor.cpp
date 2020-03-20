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
    Serial.print("ArraySize = ");
    Serial.println(this->get_data_n());
#endif
    for (int i = 0; i < this->get_data_n(); i++)
    {
        this->dataArray[i].topic = MQTT_TOPIC;
        this->dataArray[i].topic += this->getTopic();
#ifdef DEBUG
        Serial.println(this->dataArray[i].topic);
#endif
    }
}

// Sensor loop, the action method will be called every SENSOR_UPDATE_INTERVAL ms
void sensor::loop()
{
    unsigned long now = millis();
    if (now - this->lastMsgTime > this->get_sensor_update_interval())
    {
        action();
        sendData(now);
        this->lastMsgTime = now;
    }
}

void sensor::sendData(unsigned long time)
{
    for (int i = 0; i < this->get_data_n(); i++)
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