#include "mqtt_communication.h"

MQTTCommunication::MQTTCommunication() : mqttClient(wifiClient){
    // Le constructeur initialise le client MQTT avec le client WiFi
}


// Initialize WiFi and MQTT connections

void MQTTCommunication::begin() {
    // Connexion WiFi
  Serial.print("Connexion WiFi");

  setupWifi();

  setupInitialMQTT();
}




// ------------------ Setup communications ---------------------------

void MQTTCommunication::setupWifi(){
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  delay(2000);
  Serial.println("\nWiFi connecté !");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  delay(3000);
  // récupérer l'heure NTP
  
  long epoch = WiFi.getTime();  // secondes depuis 1970
  if (epoch == 0) {
    Serial.println("Erreur récupération NTP !");
  } else {
    Serial.print("Timestamp NTP : ");
    Serial.println(epoch);
  }
  delay(1000);
}

void MQTTCommunication::setupInitialMQTT(){
    // Connexion MQTT
  // ==== Générer ID client unique à partir de la MAC ====
  uint8_t mac[6];
  WiFi.macAddress(mac); // récupère la MAC dans mac[6]
  
  // convertir MAC en chaîne hex sans séparateurs
  char macStr[13]; // 12 caractères + '\0'
  snprintf(macStr, sizeof(macStr), "%02X%02X%02X%02X%02X%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  
  snprintf(clientId, sizeof(clientId), "arduino-r4-%s", macStr);
  Serial.print("Client ID MQTT: "); Serial.println(clientId);

  mqttClient.setId(clientId);
  mqttClient.setUsernamePassword(mqtt_username, mqtt_password); // Set MQTT credentials if needed
  mqttClient.setCleanSession(false); // Keep session to receive messages sent while offline

  setupMQTT();
}


void MQTTCommunication::setupMQTT(){
    Serial.println("Connecting MQTT...");
    while (!mqttClient.connect(broker, port)) {
    Serial.print(".");
    delay(1000);
    }
    Serial.println("MQTT connection established !");

    String actionAdviceTopic = String(topic_location) + "/AQI-action-advice";
    mqttClient.subscribe(actionAdviceTopic.c_str(), 1); // Subscribe to action advice topic

}




// ------------------ Verify and keep communications ---------------------------

void MQTTCommunication::keepCommunication(){
    keepWiFiConnection();
    keepMqttConnection();
}

void MQTTCommunication::keepWiFiConnection(){
    if (WiFi.status() != WL_CONNECTED) {
        Serial.print("Wifi status: ");
        Serial.println(WiFi.status());
        Serial.println("WiFi déconnecté, tentative de reconnexion...");
        while (WiFi.begin(ssid, password) != WL_CONNECTED) {
            delay(1000);
            Serial.print(".");
        }
        Serial.println("\nReconnecté au WiFi !");
    }
}

void MQTTCommunication::keepMqttConnection(){
    if (!mqttClient.connected()) {
        mqttClient.connectError(); // code d'erreur MQTT
        Serial.println("MQTT déconnecté, code d'erreur: "); // ecrire le code d'erreur MQTT dans les logs
        Serial.println(mqttClient.connectError());

        setupMQTT();
    }

    mqttClient.poll();  // essentiel pour keepalive
    
}





// ----------------------- MQTT send functions ---------------------------

void MQTTCommunication::SendHumidity(unsigned long timestamp, float humidity) {
    // Set the topic to "humidity/room-1" for humidity messages
    char humidityTopic[50];
    snprintf(humidityTopic, sizeof(humidityTopic), "%s/humidity", topic_location);

    // Publish the humidity value to the MQTT broker
    mqttClient.poll();

    mqttClient.beginMessage(humidityTopic, false, 0);
    mqttClient.print('<');
    mqttClient.print("Sensor:");
    mqttClient.print(sensor_id);
    mqttClient.print(";Humidity:");
    mqttClient.print(humidity);
    mqttClient.print(";Timestamp:");
    mqttClient.print(timestamp);
    mqttClient.print('>');
    mqttClient.endMessage();
    Serial.println("Humidity message sent!");
}

void MQTTCommunication::SendTemperature(unsigned long timestamp, float temperature) {
    // Set the topic to "temperature/room-1" for temperature messages
    char temperatureTopic[50];
    snprintf(temperatureTopic, sizeof(temperatureTopic), "%s/temperature", topic_location);

    // Publish the temperature value to the MQTT broker
    mqttClient.poll();

    mqttClient.beginMessage(temperatureTopic, false, 0);
    mqttClient.print('<');
    mqttClient.print("Sensor:");
    mqttClient.print(sensor_id);
    mqttClient.print(";Temperature:");
    mqttClient.print(temperature);
    mqttClient.print(";Timestamp:");
    mqttClient.print(timestamp);
    mqttClient.print('>');
    mqttClient.endMessage();
    Serial.println("Temperature message sent!");
}

void MQTTCommunication::SendPressure(unsigned long timestamp, float pressure) {
    // Set the topic to "pressure/room-1" for pressure messages
    char pressureTopic[50];
    snprintf(pressureTopic, sizeof(pressureTopic), "%s/pressure", topic_location);

    // Publish the pressure value to the MQTT broker
    mqttClient.poll();

    mqttClient.beginMessage(pressureTopic, false, 0);
    mqttClient.print('<');
    mqttClient.print("Sensor:");
    mqttClient.print(sensor_id);
    mqttClient.print(";Pressure:");
    mqttClient.print(pressure);
    mqttClient.print(";Timestamp:");
    mqttClient.print(timestamp);
    mqttClient.print('>');
    mqttClient.endMessage();
    Serial.println("Pressure message sent!");
}

void MQTTCommunication::SendAirQuality(unsigned long timestamp, int air_quality) {
    // Set the topic to "cesi-strasbourg/room-1" for air quality messages
    char airQualityTopic[50];
    snprintf(airQualityTopic, sizeof(airQualityTopic), "%s/airquality", topic_location);

    // Publish the air quality value to the MQTT broker
    mqttClient.poll();

    mqttClient.beginMessage(airQualityTopic, false, 1);
    mqttClient.print('<');
    mqttClient.print("Sensor:");
    mqttClient.print(sensor_id);
    mqttClient.print(";AirQuality:");
    mqttClient.print(air_quality);
    mqttClient.print(";Timestamp:");
    mqttClient.print(timestamp);
    mqttClient.print('>');
    mqttClient.endMessage();
    Serial.println("Air quality message sent!");
}

void MQTTCommunication::SendAirQualityCategory(unsigned long timestamp, int air_quality_category) {
    // Set the topic to "cesi-strasbourg/room-1" for air quality messages
    char airQualityCategoryTopic[50];
    snprintf(airQualityCategoryTopic, sizeof(airQualityCategoryTopic), "%s/airquality", topic_location);

    // Publish the air quality value to the MQTT broker
    mqttClient.poll();

    mqttClient.beginMessage(airQualityCategoryTopic, false, 1);
    mqttClient.print('<');
    mqttClient.print("Sensor:");
    mqttClient.print(sensor_id);
    mqttClient.print(";AirQualityCategory:");
    mqttClient.print(air_quality_category);
    mqttClient.print(";Timestamp:");
    mqttClient.print(timestamp);
    mqttClient.print('>');
    mqttClient.endMessage();
    Serial.println("Air quality category message sent!");
}


void MQTTCommunication::sendDataFrames(BmeValues bme_values, int air_quality_value, int air_quality_category) {
    // timestamp use for API message received management to dont store redondant data in DB
    unsigned long sending_timestamp = WiFi.getTime();

    SendTemperature(sending_timestamp, bme_values.temperature);
    SendHumidity(sending_timestamp, bme_values.humidity);
    SendPressure(sending_timestamp, bme_values.pressure);
    SendAirQuality(sending_timestamp, air_quality_value);
    SendAirQualityCategory(sending_timestamp, air_quality_category);

    displayDataFrames(sending_timestamp, bme_values, air_quality_value, air_quality_category);
}





// ------------------------ MQTT display function ---------------------------
void MQTTCommunication::displayDataFrames(unsigned long timestamp, BmeValues bme_values, int air_quality_value, int air_quality_category) {
  Serial.print("<Sensor:");
  Serial.print(sensor_id);
  Serial.print(";Temperature:");
  Serial.print(bme_values.temperature);
  Serial.print(";Timestamp:");
  Serial.print(timestamp);
  Serial.println('>');
  Serial.print("<Sensor:");
  Serial.print(sensor_id);
  Serial.print(";Humidity:");
  Serial.print(bme_values.humidity);
  Serial.print(";Timestamp:");
  Serial.print(timestamp);
  Serial.println('>');
  Serial.print("<Sensor:");
  Serial.print(sensor_id);
  Serial.print(";Pressure:");
  Serial.print(bme_values.pressure);
  Serial.print(";Timestamp:");
  Serial.print(timestamp);
  Serial.println('>');
  Serial.print("<Sensor:");
  Serial.print(sensor_id);
  Serial.print(";AirQuality:");
  Serial.print(air_quality_value);
  Serial.print(";Timestamp:");
  Serial.print(timestamp);
  Serial.println('>');
  Serial.print("<Sensor:");
  Serial.print(sensor_id);
  Serial.print(";AirQualityCategory:");
  Serial.print(air_quality_category);
  Serial.print(";Timestamp:");
  Serial.print(timestamp);
  Serial.println('>');
}