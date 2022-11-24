# Embedded-project-course


In this project, I had made a small bluetooth device to monitoring environment sensing like: Sound loudness, temperature, humidity, pressure, air quality...
After that, a rasperry pi 4B was used as a BLE gateway to receive data from the device and using MQTT to publish and vizualize the data using Node-Red.

## Project overview:

![image](https://user-images.githubusercontent.com/63698805/203825455-a8bed558-0973-4766-89e2-432b2fd8693c.png)

## IoT device setup:

In this setup, we will make a smal device serve as an IoT sensor, actually an array of sensors. The XIAO nrf52840 MCU , comes with the powerful nRF52840 processor from Nordic Semiconductors, a 32-bit ARM Cortex-M4 CPU running at 64 MHz, 1MB of CPU Flash Memory, 256KB of SRAM, and a Bluetooth 5 low energy module.The device also contains an array of embedded sensors:
* Grove Analog microphone to monitoring sound loudness

* BME680 Adafruit sensor for relative humidity, temperature, barometer, gas resistance.

* SW 520D tilt sensor for detect if the device been moved.

* Lithium battery to power the device.


![image](https://user-images.githubusercontent.com/63698805/203826592-872adf94-b08b-40b8-a401-b69214ec21f0.png) ![316613222_531629095508170_2519224450054969624_n](https://user-images.githubusercontent.com/63698805/203827878-043f3111-1861-4fef-a013-e67faf6f060f.jpg)
## Previewing and Debugging BLE Device Services

Before looking at the code running on the Raspberry Pi, I use  nRF Connect for Mobiles to preview and debug the Environmental Sensing service running on the device and being advertised over BLE.


![316844916_1535448263587459_5619001640392204036_n](https://user-images.githubusercontent.com/63698805/203832576-641bf12c-c177-456d-8d55-cdfa22173e1b.jpg)![316606099_532851551754137_6837968930547414721_n](https://user-images.githubusercontent.com/63698805/203832944-b3cc2afd-c080-43f0-88af-46ae710af289.jpg)


## IoT gateway setup:

In this post, I will use a recent generation Raspberry Pi 4 Model B single-board computer (SBC), to serve as an IoT Gateway. The device will transmit IoT sensor telemetry, over BLE, to the Raspberry Pi. The Raspberry Pi, using Wi-Fi or Ethernet, is then able to securely transmit the sensor telemetry data to the Cloud. In Bluetooth terminology, the Bluetooth Peripheral device (aka GATT Server), which is our device, will transmit data to the Bluetooth Central device (aka GATT Client), which is the Raspberry Pi.

* BLE Client Python Code
To act as the BLE Client (aka central device), the Raspberry Pi runs a Python script. The script,  `gateway.py`, uses the bluepy Python module for interfacing with BLE devices through Bluez, on Raspberry Pi OS.

![316130815_839430733934008_3642109668385598030_n](https://user-images.githubusercontent.com/63698805/203835482-cf2be516-5163-432a-a7a7-eb536b00e0f6.png)

## Node-Red Dashboard(not complete):

* Preview:

![312012928_3248604738727880_330571658152287546_n](https://user-images.githubusercontent.com/63698805/203835665-86f69f0a-0329-4fd4-9e0a-06a45b4245e2.png)


## Update incoming:

-Project Report

-Real PCB

-Node Red Dashboard
