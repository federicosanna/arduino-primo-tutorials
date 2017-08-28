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

![small_screenshot_20170727-154627](https://user-images.githubusercontent.com/30337324/29778300-2e24bb72-8c0f-11e7-9167-d8cbc2df4bb7.png)

Open the HRM tool.

![small_screenshot_20170727-154636](https://user-images.githubusercontent.com/30337324/29778464-c10f3c50-8c0f-11e7-9cf0-460b26cb8011.png)

Click **connect** button 

![small_screenshot_20170727-154647 1](https://user-images.githubusercontent.com/30337324/29778583-30a29738-8c10-11e7-9b58-181fbb8cd829.png)

Select “**HeartRateSketch**” to connect to your board.

![small_screenshot_20170727-154837](https://user-images.githubusercontent.com/30337324/29779575-3c3d018e-8c13-11e7-80ff-8a79f242edc4.png)

You should see your heart rate being recorded and displayed over time on the app.

Once the board is connected via bluetooth, you can open the serial plotter from Arduino IDE (Tools->Serial Plotter) 
to visualise your heart beats on the screen.

![screen shot 2017-07-27 at 2 47 25 pm](https://user-images.githubusercontent.com/30337324/29779776-e0f4c3a6-8c13-11e7-9a44-6dfac00cef87.png)

