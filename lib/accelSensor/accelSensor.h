#include <sensor.h>
#include <Wire.h>

#define HOSTNAME "AccelSensor"

#define SENSOR_TOPIC_NAME "accel/"

#define MPU_ADDR 0x68

class accelSensor : public sensor
{
private:
    
public:
    accelSensor(PubSubClient client);
    void setup();
    void action();
    char *getTopic();

    int get_data_n() {return 7;};
    int get_sensor_update_interval() {return 10;};
};
