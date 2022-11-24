#include <ArduinoBLE.h>
#include "Adafruit_BME680.h"
#define HICHG 22  
#define BATR P0_14  
#define buzzer 9  
#define Gas_UUID "1be9c3a1-d3da-471b-8424-449a919af977"
#define Sound_UUID "782dcb14-2059-4e04-9baa-156188530999"
#define Tilt_UUID "ac5e30ce-ab84-42dd-ab0e-ed5c3903d0ee"

Adafruit_BME680 bme; // I2C

int previousTemperature = 0;
unsigned int previousHumidity = 0;
unsigned int previousPressure = 0;
unsigned int previousGas = 0;
unsigned int previousSound = 0; 
const double vRef = 3.3; // Assumes 3.3V regulator output is ADC reference voltage
const unsigned int numReadings = 1024; // 10-bit ADC readings 0-1023, so the factor is 1024
long previousMillis = 0; // last time readings were checked, in ms
const int UPDATE_FREQUENCY = 5000;     // Update frequency in m

BLEService environmentService("181A"); // Standard Environmental Sensing service

BLEService batteryService("180F"); // Battery servivce

BLEUnsignedCharCharacteristic batteryLevelChar("2A19", BLERead | BLENotify);

BLEIntCharacteristic tempCharacteristic("2A6E",BLERead | BLENotify); 

BLEUnsignedIntCharacteristic humidCharacteristic("2A6F",BLERead | BLENotify);

BLEUnsignedIntCharacteristic pressureCharacteristic("2A6D",BLERead | BLENotify); 

BLEUnsignedIntCharacteristic gasCharacteristic(Gas_UUID,BLERead | BLENotify);

BLEUnsignedIntCharacteristic soundCharacteristic(Sound_UUID,BLERead | BLENotify);

BLEIntCharacteristic tiltCharacteristic(Tilt_UUID,BLERead | BLENotify);

void  init_sensor (Adafruit_BME680& bme) {
  bme.setTemperatureOversampling (BME680_OS_8X);
  bme.setHumidityOversampling (BME680_OS_2X);
  bme.setPressureOversampling (BME680_OS_4X);
  bme.setIIRFilterSize (BME680_FILTER_SIZE_3);
  bme.setGasHeater ( 320 , 150 ); // 320*C for 150 milliseconds
}

void setup() {
  Serial.begin(115200);

  if (!bme.begin()) {
    while (1);
  }
  init_sensor (bme);
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode (HICHG, OUTPUT);  digitalWrite(HICHG, LOW); // HIGH Charging mode
  pinMode(BATR, OUTPUT);   digitalWrite(BATR, LOW); // Set pin P0_14 LOW to read BAT IN PIN 32
  
  if (!BLE.begin()) {
    while (1);
  }
  BLE.setLocalName("nrf52840");    // Set name for connection
  BLE.setAdvertisedService(environmentService); // Advertise environment service
  BLE.setAdvertisedService(batteryService); 
  
  batteryService.addCharacteristic(batteryLevelChar); 
  environmentService.addCharacteristic(tempCharacteristic);     // Add temperature characteristic
  environmentService.addCharacteristic(humidCharacteristic);    // Add humidity characteristic
  environmentService.addCharacteristic(pressureCharacteristic); // Add pressure characteristic
  environmentService.addCharacteristic(gasCharacteristic);      // Add gas characteristic
  environmentService.addCharacteristic(soundCharacteristic);    // Add sound characteristic
  environmentService.addCharacteristic(tiltCharacteristic);    // Add tilt characteristic
 
  BLE.addService(batteryService); 
  BLE.addService(environmentService); // Add environment service
  
  tempCharacteristic.setValue(0);     // Set initial temperature value
  humidCharacteristic.setValue(0);    // Set initial humidity value
  pressureCharacteristic.setValue(0); // Set initial pressure value
  gasCharacteristic.setValue(0);      // Set initial gas value
  soundCharacteristic.setValue(0);    // Set initial sound value
  tiltCharacteristic.setValue(0);     // Set initial tilt value
  batteryLevelChar.writeValue(0);
  
  BLE.advertise(); // Start advertising
  Serial.println("Bluetooth® device active, waiting for connections...");
}


void loop() {
   BLEDevice central = BLE.central(); 
   if (central) {
        Serial.print("Connected to central MAC: ");
        Serial.println(central.address()); // Central's BT address:

        digitalWrite(LED_BUILTIN, HIGH); // Turn on the LED to indicate the connection

        while (central.connected()) {
            long currentMillis = millis();
            // After UPDATE_FREQUENCY ms have passed, check temperature & humidity
            if (currentMillis - previousMillis >= UPDATE_FREQUENCY) {
                previousMillis = currentMillis;
                updateReadings();
                updateBatteryLevel();
                alarm();
            }
        }

        digitalWrite(LED_BUILTIN, LOW); // When the central disconnects, turn off the LED
        Serial.print("Disconnected from central MAC: ");
        Serial.println(central.address());
    }
}

void updateReadings() {
    int temperature = bme.readTemperature() * 100;
    unsigned int humidity = bme.readHumidity() * 100;
    unsigned int pressure = bme.readPressure();
    unsigned int gas = bme.readGas();
    unsigned int sound = (analogRead(A0) + 83.2073) /3.503;
    int tilt = digitalRead(8);
    bool tiltChanged = (tiltCharacteristic.value() != tilt);
    
    if (temperature != previousTemperature) { // If reading has changed
        Serial.print("Temperature: ");
        Serial.println(temperature);
        tempCharacteristic.writeValue(temperature); // Update characteristic
        previousTemperature = temperature;          // Save value
    }

    if (humidity != previousHumidity) { // If reading has changed
        Serial.print("Humidity: ");
        Serial.println(humidity);
        humidCharacteristic.writeValue(humidity);
        previousHumidity = humidity;
    }

    if (pressure != previousPressure) { // If reading has changed
        Serial.print("Pressure: ");
        Serial.println(pressure);
        pressureCharacteristic.writeValue(pressure);
        previousPressure = pressure;
    }
    
    if (gas != previousGas) { // If reading has changed
        Serial.print("Gas: ");
        Serial.println(gas);
        gasCharacteristic.writeValue(gas);
        previousGas = gas;
    }
    
    if (sound != previousSound) { // If reading has changed
        Serial.print("Sound(dB): ");
        Serial.println(sound);
        soundCharacteristic.writeValue(sound);
        previousSound = sound;
    }

    if (tiltChanged) { // If reading has changed
        Serial.print("Tilt: ");
        Serial.println(tilt);
        tiltCharacteristic.writeValue(tilt);
        delay(50);
    }
}

void updateBatteryLevel() {
  unsigned int adcCount = analogRead(PIN_VBAT);
  double adcVoltage = (adcCount * vRef) / numReadings;
  double vBat =(adcVoltage*1550.0/510.0) *100; // Voltage divider from Vbat to ADC
  unsigned int BatteryLevel;
  
  if( 4 <= vBat){
    BatteryLevel= 100;
    batteryLevelChar.writeValue(BatteryLevel); 
    Serial.print("Battery Level % is now: "); // print it
    Serial.println(BatteryLevel);
  }  
  else if( 3.93<= vBat <= 3.97){
    BatteryLevel= 90;
    batteryLevelChar.writeValue(BatteryLevel); 
  }
  else if( 3.89<= vBat <= 3.93){
    BatteryLevel= 80;
    batteryLevelChar.writeValue(BatteryLevel); 
  }
  else if( 3.85<= vBat <= 3.89){
    BatteryLevel= 70;
    batteryLevelChar.writeValue(BatteryLevel); 
  }
  else if( 3.82<= vBat <= 3.85){
    BatteryLevel= 60;
    batteryLevelChar.writeValue(BatteryLevel); 
  }
  else if( 3.79<= vBat <= 3.82){
    BatteryLevel= 50;
    batteryLevelChar.writeValue(BatteryLevel); 
  }
  else if( 3.75<= vBat <= 3.79){
    BatteryLevel= 40;
    batteryLevelChar.writeValue(BatteryLevel); 
  }
  else if( 3.72<= vBat <= 3.75){
    BatteryLevel= 30;
    batteryLevelChar.writeValue(BatteryLevel); 
  }
  else if( 3.7<= vBat <= 3.72){
    BatteryLevel= 20;
    batteryLevelChar.writeValue(BatteryLevel); 
  }
  else if( 3.65<= vBat <= 3.7){
    BatteryLevel= 10;
    batteryLevelChar.writeValue(BatteryLevel); 
  }
  else if(vBat <= 3.65){
    BatteryLevel= 0;
    batteryLevelChar.writeValue(BatteryLevel); 
  }
}

void alarm()  {
  int temp = bme.readTemperature();
  unsigned int humid = bme.readHumidity();
  unsigned int Gas = bme.readGas();
  if(temp >60 && humid <10 || Gas < 5000){
    tone(buzzer, 400, 500); //the buzzer emit sound at 400 MHz for 500 millis
    delay(500); //wait 500 millis
    tone(buzzer, 650, 500); //the buzzer emit sound at 650 MHz for 500 millis
    delay(500); //wait 500 millis
  }
}
