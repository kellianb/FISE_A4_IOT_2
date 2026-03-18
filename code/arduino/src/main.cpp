#include <SoftwareSerial.h>
#include "Config.h"
#include "mqtt_communication.h"
#include "air_quality_sensor.h"
#include "bme_sensor.h"
#include "typedefs.h"

/*------------- sensors ---------------*/

AirQualitySensor air_quality_sensor(airQualitySensorPin, stdAirQualityVoltage);
BmeSensor bme_sensor;

MQTTCommunication mqttClient(false);

// Sensor values 
BmeValues v_avg;
bool bme_values_count = false;
int air_quality_avg = 0;
bool air_quality_count = false;

// Current & last statement for threshold crossing detection
int last_statement = -1;
int current_statement = -1;
const char* current_statement_message = "NA";

// Time counting
unsigned long last_send_millis = 0;
unsigned long last_buzzer_millis = 0;


//-------------------------------- Setup -------------------------------

void setup() {
  pinMode(buzzer_pin, OUTPUT);
  Serial.begin(9600); // USB
  Serial.println("Serial initialized!");

  // Initialize communication
  mqttClient.begin(); // MQTT

  // Initialize sensors
  air_quality_sensor.init();
  bme_sensor.init();
  delay(500);

}



// ---------------------- Data retrivement & threatment functions ----------------------

/*
Count the average value of the data retrieved since last message sended
*/
void avgDataCount(BmeValues *v, int *air_quality){
  if (bme_values_count == false){
      bme_values_count = true;
      v_avg = *v;
    } else {
      v_avg.temperature = (v_avg.temperature + v->temperature) / 2;
      v_avg.humidity = (v_avg.humidity + v->humidity) / 2;
      v_avg.pressure = (v_avg.pressure + v->pressure) / 2;
    }

    if (air_quality_count == false){
      air_quality_count = true;
      air_quality_avg = *air_quality;
    } else {
      air_quality_avg = (air_quality_avg + *air_quality) / 2;
    }
}

/*
Sending data to MQTT broker
*/
void sendMqtt(bool threshold, int air_quality = 0){
  Serial.println("Sending tram to MQTT broker");
  if (threshold){
    mqttClient.sendDataFrames(v_avg, air_quality, current_statement);
  } else {
    int air_quality_cat_avg = air_quality_sensor.getStatement(air_quality_avg);
    mqttClient.sendDataFrames(v_avg, air_quality_avg, air_quality_cat_avg);
  }
  last_send_millis = millis();
  bme_values_count = false;
  air_quality_count = false;
}

/*
Sound buzzer
*/
void soundBuzzer() {
  last_buzzer_millis = millis();
  for (int i = 1000; i <= 1200; i += 200) {
    tone(buzzer_pin, i, 200); // génère un son à fréquence i pendant 200 ms
    delay(250);               // petit délai entre les sons
  }
}





// ---------------------- Main loop ---------------------------

void loop() {
  // Add button interaction to change display mode (air quality statement, temperature, humidity, pressure)
  
  BmeValues v = bme_sensor.getValues();
  int air_quality = air_quality_sensor.getValue();

  // Count average
  avgDataCount(&v, &air_quality);

  // Check statement of air quality
  int statement = air_quality_sensor.getStatement(air_quality);

  // Send MQTT on new statement and alert if quality is very bad (force signal)
  if (statement != current_statement) {
    Serial.print("Air quality statement: ");
    Serial.println(air_quality_sensor.getStatementMessage(statement));

    if (statement == AirQualitySensor::FORCE_SIGNAL) {
      last_buzzer_millis = millis();
      soundBuzzer();
    }
    last_statement = current_statement;
    current_statement = statement;
    current_statement_message = air_quality_sensor.getStatementMessage(statement);
    sendMqtt(true, air_quality);
  }
  

  // Send data to MQTT broker every 5 minutes - 1 minute for demo and testing
  if (millis() - last_send_millis > 60000){
    sendMqtt(false);
  }

  // Alert if air quality stay very bad (force signal) every 5 minutes
  if (current_statement == AirQualitySensor::FORCE_SIGNAL) {
    if (millis() - last_buzzer_millis > 300000) {
      soundBuzzer();
    }
  }


  // Verify wifi & mqtt connection
  mqttClient.keepCommunication();
}