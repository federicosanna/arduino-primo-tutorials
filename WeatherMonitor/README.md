# Weather Monitor

Level: *Advanced*

### Description

This example makes use of two boards, an Arduino Primo and an Arduino Primo Core.   
The two boards communicate via Bluetooth, Primo is the Peripheral and Primo Core is Central.  
When Primo Core sends a signal (a simple byte) to Primo, it searches on internet the current temperature
on a given city (which in this example is Cagliari, Italy) requesting it to the New York Time server.  
At the end of the reserch the temperature in Celsius is sent back to Primo Core, from which the temperature 
from the server and the ambiental temperature from the sensor, can be read via NFC with a smartphone or another 
NFC reader.  
Demonstrate the use of BLE, NFC, WiFi, and how to make a Primo and a Primo Core communicate between each other,
making use of the features present on both (BLE and NFC), the ones present on the Primo only (Wifi), and the ones present only on the Primo Core (temperature sensor).

### Hardware

- [Arduino Primo](http://www.arduino.org/products/boards/arduino-primo)
- [Arduino Primo Core](http://www.arduino.org/products/boards/arduino-primo-core)
- Coin battery CR2032
- Smartphone with NFC enabled

### Software

- [nRF Toolbox](https://www.nordicsemi.com/eng/Products/Nordic-mobile-Apps/nRF-Toolbox-App)
- [Arduino IDE](http://www.arduino.org/downloads)

### Code

Refer to the file : **DigitalPinBoard.ino** in this folder

### Warning

Use the *Arduino IDE 1.8.x* and check that you have already downloaded the corresponding platform (*Arduino NRF52 Boards*) 
from the **Board Manager**.

### Output

After connecting your smartphone to the board, it will be possible to update one of its caracteristics via BLE.
The BLE transmission via the Nordic App will allow to send only 20 characters at time, so the messages you can send
will be pretty short, but if you are interested you can easily implement the code to overcome this problem.
Once you upload the code you can bring your phone (or another NFC reader) close to the NFC antenna on the board, and the message you 
uploaded will be visible on the display.
