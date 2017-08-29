# Weather Monitor

Level: *Advanced*

### Description

This example makes use of two boards, an Arduino Primo and an Arduino Primo Core.   
The two boards communicate via Bluetooth, Primo is the Peripheral and Primo Core is Central.  
When Primo Core sends a signal (a simple byte) to Primo, it searches on internet the current temperature
on a given city (which in this example is Cagliari, Italy) requesting it from the New York Time server.  
At the end of the reserch the temperature in Celsius is sent back to Primo Core, from which the temperature 
from the server and the environment temperature from the sensor, can be read via NFC with a smartphone or another 
NFC reader.  
Demonstrate the use of BLE, NFC, WiFi, and how to make a Primo and a Primo Core communicate between each other,
making use of the features present on both (BLE and NFC), the ones present on the Primo only (Wifi), and the ones present only on the Primo Core (temperature sensor).

### Hardware

- [Arduino Primo](http://www.arduino.org/products/boards/arduino-primo)
- [Arduino Primo Core](http://www.arduino.org/products/boards/arduino-primo-core)
- Coin battery CR2032
- Smartphone with NFC enabled

### Software

- [Arduino IDE](http://www.arduino.org/downloads)

### Code

Refer to the files in this folder. 
The file **TemperatureComparatorCentral.ino** should be uploaded on the Primo Core. 
The file **GetTemperatureOnlinePeripheral.ino** should be uploaded on the Primo.

### Warning

Use the *Arduino IDE 1.8.x* and check that you have already downloaded the corresponding platform (*Arduino NRF52 Boards*) 
from the **Board Manager**.

### Output

Passing the phone on the NFC antenna of the Primo Core you will receive informarmation about the current temperature in 
the city you set and information about the environment temperature.
If you would like to chance city, you can do it from the code **GetTemperatureOnlinePeripheral.ino**.

