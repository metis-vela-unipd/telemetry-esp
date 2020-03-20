#include <accelSensor.h>

accelSensor::accelSensor(PubSubClient client)
: sensor(client)
{
    this->dataArray = new sensorData[this->get_data_n()];
}

void accelSensor::setup()
{
    sensor::setup();

#ifdef DEBUG
    Serial.println("Accel Sensor setup occurring!");
#endif

    Wire.begin();
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0);    // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);

#ifdef DEBUG
    Serial.println("Wire setup done");
#endif

    this->dataArray[0].topic += "AcX";
    this->dataArray[1].topic += "AcY";
    this->dataArray[2].topic += "AcZ";
    this->dataArray[3].topic += "Tmp";
    this->dataArray[4].topic += "GyX";
    this->dataArray[5].topic += "GyY";
    this->dataArray[6].topic += "GyZ";

#ifdef DEBUG
    Serial.println("Topic added");
#endif
}

void accelSensor::action()
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 14, true); // request a total of 14 registers
    this->dataArray[0].value = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    this->dataArray[1].value = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    this->dataArray[2].value = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    this->dataArray[3].value = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    this->dataArray[4].value = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    this->dataArray[5].value = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    this->dataArray[6].value = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

char* accelSensor::getTopic()
{
    return SENSOR_TOPIC_NAME;
}