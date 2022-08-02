#!/usr/bin/python
import sys
import Adafruit_DHT
from paho.mqtt import client as mqtt_client

broker = '127.0.0.1' 
port = 1883
topic = "/1/temperatura"
topic2 = "/1/humedad"
client_id = f'python-mqtt-{random.randint(0, 1000)}'
username = 'usuario'
password = '12345'

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Conectado!")
        else:
            print("echo $(date)'Error conexion'>> /var/log/ErrorConexion.txt")
    # Set Connecting Client ID
    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

def publish(client):
    while True:
        humidity, temperature = Adafruit_DHT.read_retry(11, 4)
        time.sleep(300)
        msg = f"{0:0.1f}  " 
        msg2 = f"{1:0.1f} "
        result = client.publish(topic, msg) and client.publish(topic2, msg2)

        # result: [0, 1]
        status = result[0]
        if status == 1:
            print(f"echo $(date)'Error enviando el mensaje'>> /var/log/ErrorSensor.txt")
        

