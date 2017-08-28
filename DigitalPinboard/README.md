# Digital Pinboard

Level: *Advanced*

### Description

This example shows how to use Arduino Primo as a Pinboard. By mean of a Smartphone, the message stored in the board can be updated via BLE and can be read by NFC.
To upload the message you will need to install on your smartphone the ([nRF Toolbox app](https://www.nordicsemi.com/eng/Products/Nordic-mobile-Apps/nRF-Toolbox-App)).

### Hardware

- [Arduino Primo](http://www.arduino.org/products/boards/arduino-primo)
- Smartphone with bluetooth and NFC enabled

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
