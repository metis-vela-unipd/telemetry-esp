#include <Arduino.h>
#include <PubSubClient.h>

/*
General sensor class, just a template for all sensor types 
*/
#define MSG_BUFFER_SIZE 50

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
    sensorData* dataArray;

    sensor(PubSubClient client);

    void setup();
    void loop();
    void sendData(unsigned long time);

    // Virtual methods that will be defined in each different sensor
    virtual char *getTopic() = 0;
    virtual void action() = 0;

    virtual int get_data_n() = 0;
    virtual int get_sensor_update_interval() = 0;
};
