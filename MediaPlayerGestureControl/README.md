# Media Player Gesture Control

Level: *Advanced*

### Description

This example shows how to use the sensors on the **Arduino Primo Core** to play music on your smartphone or laptop. 
It uses the [BLE library](http://www.arduino.org/learning/reference/blehttp://www.arduino.org/learning/reference/ble) 
to send information to the device and allows you to control the music remotely.

### Hardware

- [Arduino Primo Core](http://www.arduino.org/products/boards/arduino-primo-core)
- Coin battery CR2032
- Smartphone with Bluetooth enabled

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

