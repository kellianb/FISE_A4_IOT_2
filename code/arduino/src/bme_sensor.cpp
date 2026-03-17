#include "bme_Sensor.h"

BmeSensor::BmeSensor()
    {
    // do nothing
}

bool BmeSensor::init(void) {
  Wire.begin();

  delay(500);

  if(!bme280_sensor.begin(0x76, &Wire))
  {
    Serial.println("Could not find a valid BME");
    while(1);
  }
  Serial.println("BME initialized successfully!");

  bme_temp = bme280_sensor.getTemperatureSensor();
  bme_pressure = bme280_sensor.getPressureSensor();
  bme_humidity = bme280_sensor.getHumiditySensor();

  bme_temp->printSensorDetails();
  bme_pressure->printSensorDetails();
  bme_humidity->printSensorDetails();
}

/*
Extract the bme values
*/
BmeValues BmeSensor::getValues() {
  BmeValues v;
  v.temperature = bme280_sensor.readTemperature();
  delay(5); // attendre que la conversion suivante soit stable
  v.humidity    = bme280_sensor.readHumidity();
  delay(5); // attendre que la conversion suivante soit stable
  v.pressure    = bme280_sensor.readPressure() / 100.0F;
  return v;
}