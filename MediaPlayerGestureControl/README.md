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

### Circuit

It doesn’t need any circuit but it need an [Arduino Primo](http://www.arduino.org/products/boards/arduino-primo) 
or an external programmer only  for uploading the sketch. 
For more information, about how to upload a sketch, visit the [Getting Started](http://www.arduino.org/learning/getting-started/getting-started-with-arduino-primo-core).

### Code

Refer to the file : **MediaPlayerGestureControl.ino** in this folder

### Warning

Use the *Arduino IDE 1.8.x* and check that you have already downloaded the corresponding platform (*Arduino NRF52 Boards*) 
from the **Board Manager**.

### Output

After connecting your smartphone (or another device to play music) to the board, you will be able to play media with 
your board.
Completely rotate the board downward by rotating it to the right to play orpause music. 
Tilt it to the right to play the next track, to the left to playthe previous track. 
To increase/decrease the volume, tilt it forward/backward.

