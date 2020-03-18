#include <Arduino.h>
#include <PubSubClient.h>

/*
General sensor class, just a template for all sensor types 
*/

#define SENSOR_UPDATE_INTERVAL 100 //ms
#define MSG_BUFFER_SIZE 50

#define SENSOR_DATA_N 2

#define MQTT_TOPIC "sensor/"

// Struct to store the sensor data values
struct sensorData
{
    String topic;
    unsigned int value;    
};


class sensor
{
private:
    PubSubClient client;
    unsigned long lastMsgTime = 0;
public:
    sensorData dataArray[SENSOR_DATA_N];

    sensor(PubSubClient client);

    void setup();
    void loop();
    void sendData(unsigned long time);


    // Virtual methods that will be defined in each different sensor
    virtual char* getTopic(){ return ""; };
    virtual void action(){};
};
