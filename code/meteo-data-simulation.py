import paho.mqtt.client as mqtt
import random
import time

brocker = "localhost"
port = 8883
username = "arduino-room-2"
password = "!i0lmde32@.de22-22"

sensors_list = ["arduino-room-2", "arduino-room-3", "arduino-room-4"]
username_list = ["arduino-room-2", "arduino-room-3", "arduino-room-4"]
password_list = ["!i0lmde32@.de22-22", "!i0lmde32@.de23-23", "!i0lmde32@.de24-24"]
topic_list = ["cesi-strasbourg/building-1/room-102", "cesi-strasbourg/building-2/room-223", "cesi-strasbourg/building-1/room-114"]
################## Simulate sensor data generation ##################

def random_temperature():
    """_summary_
        generate random temperature value between 15 and 30 degrees Celsius
    Returns:
        _type_: float
    """
    return round(random.uniform(15, 30), 2)


def random_humidity():
    """_summary_
        generate random humidity value between 40 and 80 percent
    Returns:
        _type_: float
    """
    return round(random.uniform(40, 80), 2)


def random_pressure():
    """_summary_
        generate random pressure value between 990 and 1020 hPa
    Returns:
        _type_: float
    """
    return round(random.uniform(990, 1020), 1)


def random_air_quality():
    """_summary_
        generate random air quality value between 0 and 500
    Returns:
        _type_: int
    """
    return random.randint(1, 75)

    
std_air_quality_voltage = 25


def getRatio(voltage):
    global std_air_quality_voltage
    """_summary_
            calculate the ratio of the current voltage to the standard air quality voltage
            and return the percentage value
    Args:
        voltage (int): the current voltage value from the sensor (simulated as air quality value)

    Returns:
        float: the percentage value of the ratio
    """
    ratio = (voltage - std_air_quality_voltage) / std_air_quality_voltage * 100.0
    return ratio


def statement(voltage):

  ratio = getRatio(voltage)
  if (ratio > 70.0):
      #Force signal, if the air quality is very bad, to trigger the alarm
      return 0
  elif (ratio > 50.0):
      #High pollution, if the air quality is bad, to trigger the alarm
      return 1
  elif (ratio > 30.0):
      #Medium pollution, if the air quality is not good, to trigger the alarm
      return 2
  else:
      #Fresh air, if the air quality is good
      return 3

  return -1

def send_message(client, topic, payload):
    """_summary_
        send a message to the MQTT broker with the given topic and payload
    Args:
        client (mqtt.Client): the MQTT client instance
        topic (str): the topic to publish the message to
        payload (str): the message payload to be sent
    """
    result = client.publish(topic, payload, qos=1)
    if result[0] == mqtt.MQTT_ERR_SUCCESS:
        print(f"{time.time()}: Message published successfully to topic {topic}")
    else:
        print(f"{time.time()}: Failed to publish message to topic {topic}, error code: {result[0]}")


mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttc.username_pw_set(username, password)
mqttc.connect(brocker, port, 60)
mqttc.loop_start()


while True:
    timestemp = int(time.time())
    for i in range(len(sensors_list)):
        username = username_list[i]
        password = password_list[i]
        topic = topic_list[i]
        
        temperature = random_temperature()
        humidity = random_humidity()
        pressure = random_pressure()

        voltage = random_air_quality()
        air_category = statement(voltage)

        payload_humidity = f"{{\"sensor-id\":\"{username}\",\"timestamp\":{timestemp},\"value\":{humidity}}}"
        send_message(mqttc, topic + "/humidity", payload_humidity)
        payload_temperature = f"{{\"sensor-id\":\"{username}\",\"timestamp\":{timestemp},\"value\":{temperature}}}"
        send_message(mqttc, topic + "/temperature", payload_temperature)
        payload_pressure = f"{{\"sensor-id\":\"{username}\",\"timestamp\":{timestemp},\"value\":{pressure}}}"
        send_message(mqttc, topic + "/pressure", payload_pressure)
        payload_air_quality = f"{{\"sensor-id\":\"{username}\",\"timestamp\":{timestemp},\"value\":{voltage},\"category\":{air_category}}}"
        send_message(mqttc, topic + "/airquality", payload_air_quality)
    time.sleep(10)
mqttc.loop_stop()
mqttc.disconnect()