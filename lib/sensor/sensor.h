#include <Arduino.h>
#include <PubSubClient.h>

/*
General sensor class, just a template for all sensor types 
*/

#define SENSOR_UPDATE_INTERVAL 100 //ms

class sensor
{
public:
    PubSubClient client;

    sensor(PubSubClient client){
      this->client = client;
    }

    virtual void setup() {};
    virtual void loop() {};
    virtual void sendData(unsigned long time) {};
};