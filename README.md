# telemetry-esp

Repository for the code on the ESP-8266 responsible to interface with the various sensors:
 - Wind Vane and anemometer
 - Accelerometer
 - Strain gauges
 
## How to use

This repo is used in conjuction with [PlatformIO:https://platformio.org/].
There are three separate environments in the platformio.ini files that dictate which sensor gets build.

## Libs

In the lib folder there are the libraries that define each sensor, all derived from the general sensor class.

