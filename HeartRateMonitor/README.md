# Heart Rate monitor via Bluetooth

Level: *Advanced*

### Description

This example shows how to display the heart beats on the serial plotter and how to send information about the heart rate 
to your smartphone via BLE using the Nordic application ([nRF Toolbox app](https://www.nordicsemi.com/eng/Products/Nordic-mobile-Apps/nRF-Toolbox-App)).

### Hardware

- Arduino Primo
- Pulse sensor heartbeat 
- Smartphone with bluetooth enabled

### Software

- [nRF Toolbox](https://www.nordicsemi.com/eng/Products/Nordic-mobile-Apps/nRF-Toolbox-App)
- [Arduino IDE](http://www.arduino.org/downloads)

### Circuit

The sensor has three pins: PWR,GND and Signal.   
Follow these connections:   
PWR pin to 3.3V;  
GND to GND;  
Signal to A0.

![primowithsensor](https://user-images.githubusercontent.com/30337324/29719464-6ac3e914-89b6-11e7-9654-ba35e0fbd4f4.png)

### Code

Refer to the file : **HeartRateMonitor.ino** in this folder

### Warning

Use the *Arduino IDE 1.8.x* and check that you have already downloaded the corresponding platform (*Arduino NRF52 Boards*) 
from the **Board Manager**.

### Output

Install and launch the [nRF Toolbox](https://www.nordicsemi.com/eng/Products/Nordic-mobile-Apps/nRF-Toolbox-App) app.



