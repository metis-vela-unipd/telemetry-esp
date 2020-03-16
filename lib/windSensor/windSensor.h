#include <sensor.h>

#define HOSTNAME "WindSensor"
#define MSG_BUFFER_SIZE 50

/*
Wind sensor class, this implements the interrupts and the logic to read the wind sensors.
*/

class windSensor : public sensor
{
private:  
    unsigned long lastMsg = 0;
    unsigned int windDirection = 0;
    static unsigned long lastSpd;
    static unsigned int windSpeed;
public:
    using sensor::sensor;
    void setup();
    void loop();

    unsigned int getWindDirection();
    static void ICACHE_RAM_ATTR windSpeedInterrupt();
    
    void sendData(unsigned long time);
};
