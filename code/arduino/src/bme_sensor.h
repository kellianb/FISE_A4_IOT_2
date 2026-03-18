#ifndef __BME_SENSOR_H__
#define __BME_SENSOR_H__

#include "Arduino.h"
#include "Config.h"
#include <Adafruit_BME280.h>
#include <Wire.h>
#include "typedefs.h"

class BmeSensor {
  public:
    BmeSensor();
    bool init(void);
    BmeValues  getValues(void);

  public:
    Adafruit_BME280 bme280_sensor;
    Adafruit_Sensor *bme_temp;
    Adafruit_Sensor *bme_pressure;
    Adafruit_Sensor *bme_humidity;
  protected:
    int  _pin;
    int _stdVoltage;
    int  _currentVoltage;
    int  _lastVoltage;
};

#endif // __BME_SENSOR_H__