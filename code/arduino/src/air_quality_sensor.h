#ifndef __AIR_QUALITY_SENSOR_H__
#define __AIR_QUALITY_SENSOR_H__

#include "Arduino.h"
#include "Config.h"

class AirQualitySensor {
  public:
    AirQualitySensor(int pin, int stdVoltage);
    bool init(void);
    int  getValue(void);
    int  getStatement(int avgVoltage);
    const char* getStatementMessage(int);

    static const int FORCE_SIGNAL; // Very bad quality alert --> open windows
    static const int HIGH_POLLUTION; // Bad quality --> warning recommend to open windows
    static const int LOW_POLLUTION; // Medium quality
    static const int FRESH_AIR; // Good air quality 

  protected:
    int  _pin;
    int _stdVoltage;
    int  _currentVoltage;
    int  _lastVoltage;

    float getRatio(int avgVoltage);
};

#endif // __AIR_QUALITY_SENSOR_H__