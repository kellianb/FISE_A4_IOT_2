#include "Air_Quality_Sensor.h"

const int AirQualitySensor::FORCE_SIGNAL   = 0;
const int AirQualitySensor::HIGH_POLLUTION = 1;
const int AirQualitySensor::LOW_POLLUTION  = 2;
const int AirQualitySensor::FRESH_AIR      = 3;

AirQualitySensor::AirQualitySensor(int pin, int stdVoltage)
    : _pin(pin), _stdVoltage(stdVoltage) {
    // do nothing
}

bool AirQualitySensor::init(void) {
    int initVoltage = analogRead(_pin);

    if (10 < initVoltage && initVoltage < 798) {
        _currentVoltage = initVoltage;
        _lastVoltage = _currentVoltage;

        Serial.println("Air quality sensor initialized successfully!");
        return true;
    } else {
        Serial.println("Error during air quality sensor intialization!");
        return false;
    }
}

int AirQualitySensor::getValue(void) {
  _lastVoltage = _currentVoltage;
  _currentVoltage = analogRead(_pin);
  return _currentVoltage;
}

int AirQualitySensor::getStatement(int voltage) {

  _lastVoltage = _currentVoltage;
  _currentVoltage = voltage;

  float ratio = AirQualitySensor::getRatio(_currentVoltage);

  if (ratio > 70.0) {
      return AirQualitySensor::FORCE_SIGNAL;
  } else if (ratio > 50.0) {
      return AirQualitySensor::HIGH_POLLUTION;
  } else if (ratio > 30.0) {
      return AirQualitySensor::LOW_POLLUTION;
  } else {
      return AirQualitySensor::FRESH_AIR;
  }

  return -1;
}

float AirQualitySensor::getRatio(int voltage){
  float ratio = ((float)(voltage - _stdVoltage) / (float)_stdVoltage) * 100.0f;
  return ratio;
}

const char* AirQualitySensor::getStatementMessage(int quality){
  if (quality == AirQualitySensor::FORCE_SIGNAL) {
    return "Very bad air quality! Force signal active.";
  } else if (quality == AirQualitySensor::HIGH_POLLUTION) {
    return "bad air quality!";
  } else if (quality == AirQualitySensor::LOW_POLLUTION) {
    return "Medium air quality!";
  } else if (quality == AirQualitySensor::FRESH_AIR) {
    return "Good air quality!";
  }
  return "NA";
}