#ifndef MQTT_COMMUNICATION_H
#define MQTT_COMMUNICATION_H

#include <WiFiS3.h>
#include <WiFiSSLClient.h>
#include <ArduinoMqttClient.h>
#include <time.h>
#include "Config.h"
#include "typedefs.h"

class MQTTCommunication {
    public:
        MQTTCommunication();
        MQTTCommunication(bool useSSL);
        void begin();
        void setupWifi();
        void setupInitialMQTT();
        void setupMQTT();
        void keepCommunication();
        void keepWiFiConnection();
        void keepMqttConnection();
        void sendDataFrames(BmeValues bme_values, int air_quality_value, int air_quality_category);
        void SendTemperature(unsigned long timestamp, float temperature);
        void SendHumidity(unsigned long timestamp, float humidity);
        void SendPressure(unsigned long timestamp, float pressure);
        void SendAirQuality(unsigned long timestamp, int air_quality, int air_quality_category);
        void displayDataFrames( unsigned long timestamp, BmeValues bme_values, int air_quality, int air_quality_category);

    private:
        char clientId[32]; // tableau pour stocker l'ID MQTT
        bool _ssl = false;
        WiFiClient wifiClient;
        WiFiSSLClient wifiSSLClient; //later with CA certificate for TLS
        MqttClient mqttClient;
};

#endif // MQTT_COMMUNICATION_H