#include <sensor.h>

#define HOSTNAME "WindSensor"

#define SENSOR_TOPIC_NAME "wind/"

#define SPEED_CONVERSION_CONSTANT 4444.8

/*
Wind sensor class, this implements the interrupts and the logic to read the wind sensors.
*/

class windSensor : public sensor
{
private:
    static unsigned long lastSpeed;
    static unsigned int windSpeed;

    unsigned int data_n = 2;
public:
    windSensor(PubSubClient client);

    void setup();
    void action();
    char *getTopic();

    int get_data_n() {return 2;};
    int get_sensor_update_interval() {return 100;};

    unsigned int getWindDirection();
    static void ICACHE_RAM_ATTR windSpeedInterrupt();
};
