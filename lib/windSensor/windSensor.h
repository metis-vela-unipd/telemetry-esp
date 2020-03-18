#include <sensor.h>

#define HOSTNAME "WindSensor"

#define SENSOR_DATA_N 2

#define SENSOR_TOPIC_NAME "wind/"

/*
Wind sensor class, this implements the interrupts and the logic to read the wind sensors.
*/

class windSensor : public sensor
{
private:  
    static unsigned long lastSpeed;
    static unsigned int windSpeed;
public:
    using sensor::sensor;
    void setup();
    void action();
    char* getTopic();

    unsigned int getWindDirection();
    static void ICACHE_RAM_ATTR windSpeedInterrupt();
    
    void sendData(unsigned long time);
};
