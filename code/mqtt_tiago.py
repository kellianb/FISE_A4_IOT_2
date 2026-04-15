import paho.mqtt.client as mqtt
import random
import time
import os 
import re
import paramiko
from time import sleep

brocker = "localhost"
port = 8883
username = "tiago"
password = "!PAL-tiago@.de291"

topic = "cesi-strasbourg/building-1/room-102/airquality"

last_statement = -1
# get mqtt topic message

def on_connect(client, userdata, flags, rc):
    print("Connected with result code", rc)
    client.subscribe(topic)

# extract statement from mqtt message
#{{\"sensor-id\":\"{username}\",\"timestamp\":{timestemp},\"value\":{voltage},\"category\":{statement}}}
# statement = 0: air quality is very bad
# statement = 1: air quality is bad
# statement = 2: air quality is medium
# statement = 3: air quality is good
def get_statement(payload):
    match = re.search(r'\"category\":(\d)', payload)
    if match:
        return int(match.group(1))
    else:
        print("No category found in payload")
        return -1
    
    
def generate_message(statement):
    if statement == 0:
        message = "The air quality in the room is very bad, please open the windows for ventilation"
        return message
    elif statement == 1:
        message = "Be careful, the air quality in the room is bad, consider opening the windows for ventilation"
        return message
    elif statement == 2:
        message = "You currently have a correct air quality in the room, but it could be better"
        return message
    elif statement == 3:
        message = "You have a very good air quality in the room, keep it up!"
        return message


def on_message(client, userdata, msg):
    global last_statement
    payload = msg.payload.decode()
    print(f"Message reçu: {payload} sur {msg.topic}")
    
    statement = get_statement(payload)
    if statement != last_statement:
        print(f"Changement de qualité de l'air: {statement}")
        last_statement = statement
        message = generate_message(statement)
        print(message)
        client = paramiko.SSHClient()
        client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        client.connect('tiago-201c', username='pal', password='pal')

        stin, stout, stderr = client.exec_command(fr"rostopic pub /tts/goal pal_interaction_msgs/TtsActionGoal\"{{goal: {{rawtext: {{text: '{message}', lang_id: 'fr_FR'}}}}}}\"")
        sleep(1)
        client.close()

mqttc = mqtt.Client()
mqttc.username_pw_set(username, password)

mqttc.on_connect = on_connect
mqttc.on_message = on_message

mqttc.connect(brocker, port)
mqttc.subscribe(topic)
mqttc.loop_forever()

