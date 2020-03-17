#include <sensor.h>

#define HOSTNAME "WindSensor"

/*
Wind sensor class, this implements the interrupts and the logic to read the wind sensors.
*/

class windSensor : public sensor
{
private:  
    unsigned int windDirection = 0;
    static unsigned long lastSpd;
    static unsigned int windSpeed;
public:
    using sensor::sensor;
    void setup();
    void action();

    unsigned int getWindDirection();
    static void ICACHE_RAM_ATTR windSpeedInterrupt();
    
    void sendData(unsigned long time);
};
