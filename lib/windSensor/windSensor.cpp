#include <windSensor.h>

// Initialization of static variables needed for the interrupt routine
unsigned int windSensor::windSpeed = 0;
unsigned long windSensor::lastSpd = 0;

// Setup method, to be called in the setup call
void windSensor::setup()
{
    pinMode(A0, INPUT);
    pinMode(D5, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(D5), windSpeedInterrupt, RISING);
}

// Loop method to be called on each loop
void windSensor::action()
{
    this->windDirection = this->getWindDirection();
}

// Interrupt function to read the wind speed
void ICACHE_RAM_ATTR windSensor::windSpeedInterrupt()
{
    unsigned long spd = millis();
    if (spd - lastSpd >= 10)
    {
        windSpeed = spd - lastSpd;
        lastSpd = spd;
    }
}

// Wind direction function
unsigned int windSensor::getWindDirection()
{
    unsigned int dir;
    dir = analogRead(A0);
    return dir;
}

void windSensor::sendData(unsigned long time)
{
    char msg[MSG_BUFFER_SIZE];
    snprintf(msg, MSG_BUFFER_SIZE, "%1d, %1d, %1ld", windSpeed, windDirection, time);
    this->client.publish("WindInfo", msg);
}
