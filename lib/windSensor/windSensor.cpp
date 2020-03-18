#include <windSensor.h>

// Initialization of static variables needed for the interrupt routine
unsigned int windSensor::windSpeed = 0;
unsigned long windSensor::lastSpeed = 0;

// Setup method, to be called in the setup call
void windSensor::setup()
{
    sensor::setup();

    #ifdef DEBUG
        Serial.println("Started Sensor setup");
    #endif

    pinMode(A0, INPUT);
    pinMode(D5, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(D5), windSpeedInterrupt, RISING);

    #ifdef DEBUG
        Serial.println("Assign struct names:");
    #endif

    this->dataArray[0].topic += "direction";
    this->dataArray[1].topic += "speed";

    #ifdef DEBUG
        Serial.println("Finished Sensor Setup");
    #endif
}

char* windSensor::getTopic()
{
    return "wind/";
}

// Loop method to be called on each loop
void windSensor::action()
{
    this->dataArray[0].value = getWindDirection();
    this->dataArray[1].value = this->windSpeed;
}

// Interrupt function to read the wind speed
void ICACHE_RAM_ATTR windSensor::windSpeedInterrupt()
{
    unsigned long spd = millis();
    if (spd - lastSpeed >= 10)
    {
        windSpeed = spd - lastSpeed;
        lastSpeed = spd;
    }
}

// Wind direction function
unsigned int windSensor::getWindDirection()
{
    unsigned int dir;
    dir = analogRead(A0);
    return dir;
}