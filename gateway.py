import sys
import time
import paho.mqtt.client as mqtt 
from argparse import ArgumentParser
from bluepy import btle 
#4A:20:85:6E:28:46

mqttBroker ="192.168.0.103"
client = mqtt.Client("Gateway")
client.connect(mqttBroker, 1883) 

def main():
    # get args
    args = get_args()

    print("Connecting...")
    ble_gateway = btle.Peripheral(args.mac_address)

    print("Discovering Services...")
    _ = ble_gateway.services
    environmental_sensing_service = ble_gateway.getServiceByUUID("181A")
    battery_service = ble_gateway.getServiceByUUID("180F")
    
    print("Discovering Characteristics...")
    _ = environmental_sensing_service.getCharacteristics()
    _ = battery_service.getCharacteristics()

    while True:
        print("\n")
        read_temperature(environmental_sensing_service)
        read_humidity(environmental_sensing_service)
        read_pressure(environmental_sensing_service)
        read_gas(environmental_sensing_service)
        read_sound(environmental_sensing_service)
        read_tilt(environmental_sensing_service)
        read_BAT(battery_service)
        time.sleep(2) 


def byte_array_to_int(value):
    value = bytearray(value)
    value = int.from_bytes(value, byteorder="little")
    return value


def decimal_exponent_two(value):
    return value / 100


def decimal_exponent_one(value):
    return value / 10


def read_pressure(service):
    pressure_char = service.getCharacteristics("2A6D")[0]
    pressure = pressure_char.read()
    pressure = byte_array_to_int(pressure)
    pressure = decimal_exponent_one(pressure)
    client.publish("gateway/pressure", pressure)
    print(f"Barometric Pressure: {round(pressure, 2)} Pa")


def read_humidity(service):
    humidity_char = service.getCharacteristics("2A6F")[0]
    humidity = humidity_char.read()
    humidity = byte_array_to_int(humidity)
    humidity = decimal_exponent_two(humidity)
    client.publish("gateway/humidity", humidity)
    print(f"Humidity: {round(humidity, 2)}%")


def read_temperature(service):
    temperature_char = service.getCharacteristics("2A6E")[0]
    temperature = temperature_char.read()
    temperature = byte_array_to_int(temperature)
    temperature = decimal_exponent_two(temperature)
    client.publish("gateway/temperature", temperature)
    print(f"Temperature: {round(temperature, 2)}°C")
    
def read_gas(service):
    gas_char = service.getCharacteristics("1be9c3a1-d3da-471b-8424-449a919af977")[0]
    gas = gas_char.read()
    gas = byte_array_to_int(gas)
    client.publish("gateway/gas", gas)
    print(f"Gas: {round(gas)} R")
    
def read_sound(service):
    sound_char = service.getCharacteristics("782dcb14-2059-4e04-9baa-156188530999")[0]
    sound = sound_char.read()
    sound = byte_array_to_int(sound)
    client.publish("gateway/sound", sound)
    print(f"Sound: {round(sound)}dB")

def read_tilt(service):
    previousTilt = 0
    tilt_char = service.getCharacteristics("ac5e30ce-ab84-42dd-ab0e-ed5c3903d0ee")[0]
    tilt = tilt_char.read()
    tilt = byte_array_to_int(tilt)
    client.publish("gateway/tilt", tilt)
    if tilt != previousTilt:
        print("Position changed")

def read_BAT(service):
    bat_char = service.getCharacteristics("2A19")[0]
    bat = bat_char.read()
    bat = byte_array_to_int(bat)
    client.publish("gateway/battery", bat)
    print(f"Battery: {round(bat)}%")
    
def get_args():
    arg_parser = ArgumentParser(description="Gateway")
    arg_parser.add_argument('mac_address', help="MAC address of device to connect")
    args = arg_parser.parse_args()
    return args


if __name__ == "__main__":
    main()