#ifndef CONFIG_H
#define CONFIG_H

constexpr int buzzer_pin = 8;

// WiFi
#define WIFI_SSID       "iPhone de Florian" // "CESI_Iot"
#define WIFI_PASSWORD   "Florian18.?!zgi97F" // "#RO_i0t.n3t"
#define LOCAL_IP        172,20,10,10 // 192,168,0,28

// MQTT
constexpr const char* broker = "172.20.10.3"; // "192.168.0.25";
constexpr const int port = 8883;

constexpr const char* sensor_id = "arduino-room-1";

constexpr const char* mqtt_username = "arduino-room-1";
constexpr const char* mqtt_password = "!i0lmde32@.de21-21";
constexpr const char* topic_location = "cesi-strasbourg/building-2/room-201/air-data";

constexpr const uint8_t sender_mac_adress[8] = {0x00, 0x13, 0xA2, 0x00, 0x42, 0x24, 0xE4, 0x13};

constexpr int stdAirQualityVoltage = 25; //Set correct value after initialisation

constexpr int airQualitySensorPin = 0;


constexpr const char* rootCA = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDBTCCAe2gAwIBAgIUKOxe1/ibBMEOpSGS4TFm2vYHVqYwDQYJKoZIhvcNAQEL
BQAwEjEQMA4GA1UEAwwHTVFUVC1DQTAeFw0yNjAzMTcwOTMzMzlaFw0zNjAyMTMw
OTMzMzlaMBIxEDAOBgNVBAMMB01RVFQtQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IB
DwAwggEKAoIBAQDIfarlHtRnnEP4aG/ZsfzNAW6TifmxCAORIBxhHws61u8tmPmN
6s5C4UAlMRpX5eOIiXjsf7Wdovks/chxpGBHv9jcqf6xBT6iEmGudsCWJeDwNOwo
ofwsEmONUUwnFm+X8Tdmw+l+esybSyiFqsS8NJkRHFRHCbRCEScmqWDARX9J/HJf
bi0d4BFejXegpLrr4bbjaYL4/AgvRW1mXJOu8VMoFDm0YMbmZ7OOVn82KJ9EuGbT
1jlI0SyjHutcWNqKxWufgkprinBzx2DmJjo2H7FDzjO4PQzBJI8R5M16xT3ZOCG1
ZUG9Z1X1f87zRVC9XLeaKiVUWWbaz1AlVsjtAgMBAAGjUzBRMB0GA1UdDgQWBBRB
Gt6EkVeHCNFWHvkYYOq73aarozAfBgNVHSMEGDAWgBRBGt6EkVeHCNFWHvkYYOq7
3aarozAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQASZNcJYnwZ
eohBM29WsYAN7TjKt1d3+vDkBQwUjZtfJhuwephzkB9Vl/vTAc1rb6MC8hnaVLkE
GNp7G03Myi6yLMCZtPmEzhp7/T8GoXko46xF2ygE4bZPm2o3RqOhLmOkRj8nGoXK
Re3nZJPeAtxHhTCHPA0zmSMLVyD39Me3MlEVIiM5XYB+UGjRCI9n5yrjD4UCnIPb
Y7foKGrCf+EUjQC/iOB+URemYAPyHq0o+u4MmMoETd5OgVh8r/Y3Cv8PmWAOQMGM
AXzgQHbK7O7xaN4Q0K8gkaCL0k33Rui7D9lUyXxJam8WcRCC6/NUCitTWOU9/UYW
3ieum4PeW/ol
-----END CERTIFICATE-----
)EOF";

#endif // CONFIG_H