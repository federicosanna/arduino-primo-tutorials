# Media Player Gesture Control

Level: *Advanced*

### Description

This sketch shows how to to use the sensors on the **Arduino Primo Core** and the System Control [BLE library](http://www.arduino.org/learning/reference/blehttp://www.arduino.org/learning/reference/ble)
to use it as a mouse for your smartphone. 
The example explains how to move the pointer on the screen using the accelerometers of the Arduino Primo Core.

### Hardware

- [Arduino Primo Core](http://www.arduino.org/products/boards/arduino-primo-core)
- Coin battery CR2032
- Smartphone with Bluetooth enabled

### Circuit

It doesn’t need any circuit but it need an [Arduino Primo](http://www.arduino.org/products/boards/arduino-primo) 
or an external programmer only  for uploading the sketch. 
For more information, about how to upload a sketch, visit the [Getting Started](http://www.arduino.org/learning/getting-started/getting-started-with-arduino-primo-core).

### Code

Refer to the file : **MouseControlSmartphone.ino** in this folder

### Warning

Use the *Arduino IDE 1.8.x* and check that you have already downloaded the corresponding platform (*Arduino NRF52 Boards*) 
from the **Board Manager**.

### Output

After connecting your smartphone to the board, you will be able to move the mouse with your board.
Tilt the board on the direction where you want to move the pointer, tap on it once to press the mouse, 
and tap again to release.


