#include <Arduino.h>
#include <PubSubClient.h>

/*
General sensor class, just a template for all sensor types 
*/

#define SENSOR_UPDATE_INTERVAL 100 //ms
#define MSG_BUFFER_SIZE 50


class sensor
{
public:
    PubSubClient client;

    unsigned long lastMsgTime = 0;

    sensor(PubSubClient client);

    void setup(){};
    void loop(){};

    // Virtual methods that will be defined in each different sensor
    virtual void action();
    virtual void sendData(unsigned long time){};
};