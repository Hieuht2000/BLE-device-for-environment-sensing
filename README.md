# Embedded-Systems-project-course


In this project, I had made a small bluetooth device to monitoring environment sensing like: Sound loudness, temperature, humidity, pressure, air quality...
After that, a rasperry pi 4B was used as a BLE gateway to receive data from the device and using MQTT to publish and vizualize the data using Node-Red.

## Project overview:

![image](https://user-images.githubusercontent.com/63698805/204251410-c156735d-f2f0-43d6-b1c6-982e4dec66c0.png)


## IoT device setup:

In this setup, we will make a smal device serve as an IoT sensor, actually an array of sensors. The XIAO nrf52840 MCU , comes with the powerful nRF52840 processor from Nordic Semiconductors, a 32-bit ARM Cortex-M4 CPU running at 64 MHz, 1MB of CPU Flash Memory, 256KB of SRAM, and a Bluetooth 5 low energy module.The device also contains an array of embedded sensors:
* Grove Analog microphone to monitoring sound loudness

* BME680 Adafruit sensor for relative humidity, temperature, barometer, gas resistance.

* SW 520D tilt sensor for detect if the device been moved.

* Lithium battery to power the device.


![image](https://user-images.githubusercontent.com/63698805/204611551-cdb88ad5-83cf-4c16-ae90-fd6f31bf0e2d.png) ![316613222_531629095508170_2519224450054969624_n](https://user-images.githubusercontent.com/63698805/203827878-043f3111-1861-4fef-a013-e67faf6f060f.jpg)
## Previewing and Debugging BLE Device Services

Before looking at the code running on the Raspberry Pi, I use  nRF Connect for Mobiles to preview and debug the Environmental Sensing service running on the device and being advertised over BLE.


![image](https://user-images.githubusercontent.com/63698805/205026544-03d6d34b-2dba-453b-9f8a-71470658d935.png)![image](https://user-images.githubusercontent.com/63698805/205026652-1ac47443-d089-459d-9fc9-29fa919f2802.png)


## IoT gateway setup:

In this post, I will use a recent generation Raspberry Pi 4 Model B single-board computer (SBC), to serve as an IoT Gateway. The device will transmit IoT sensor telemetry, over BLE, to the Raspberry Pi. The Raspberry Pi, using Wi-Fi or Ethernet, is then able to securely transmit the sensor telemetry data to the Cloud. In Bluetooth terminology, the Bluetooth Peripheral device (aka GATT Server), which is our device, will transmit data to the Bluetooth Central device (aka GATT Client), which is the Raspberry Pi.

* BLE Client Python Code
To act as the BLE Client (aka central device), the Raspberry Pi runs a Python script. The script,  `gateway.py`, uses the bluepy Python module for interfacing with BLE devices through Bluez, on Raspberry Pi OS.

![316130815_839430733934008_3642109668385598030_n](https://user-images.githubusercontent.com/63698805/203835482-cf2be516-5163-432a-a7a7-eb536b00e0f6.png)

## Node-Red Dashboard:

* Preview:

![image](https://user-images.githubusercontent.com/63698805/204251594-18e62808-5be5-42a1-9f0c-c30fa68f8ecb.png)

![image](https://user-images.githubusercontent.com/63698805/204251677-afcbfd2f-234c-4786-a005-340a6bf14b39.png)



## Update incoming:
-Real PCB

-Node Red Dashboard
