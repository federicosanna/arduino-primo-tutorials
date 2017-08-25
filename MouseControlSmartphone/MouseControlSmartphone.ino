/*   
     MouseControlSmartphone.ino

     Written by Federico Sanna

     This example shows how to to use the sensors on the Arduino Primo Core
     and the System Control BLE library to use it as a mouse for your smartphone. 
     The example explains how to move the pointer on the screen making use of
     the accelerometers of the Arduino Primo Core. 
     Tilt the board on the direction where you want to move the pointer, 
     tap on it once to press the mouse, and tap again to release.

*/

#include <CoreSensors.h>
#include <BLEHIDPeripheral.h>
#include <BLEMouse.h>
#include <BLESystemControl.h>

//define mouse range
#define MOUSE_RANGE 24
//define interut pin
#define INT1 21

int tap = 0;
int crono = 0;
int count = 0;

// create peripheral instance, see pinouts above
BLEHIDPeripheral bleHIDPeripheral = BLEHIDPeripheral();
BLEMouse bleMouse;
BLESystemControl bleSystemControl;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, OUTPUT);

  // start serial port at 115200 bps
  Serial.begin(115200);

  Serial.println(F("BLE HID Peripheral - clearing bond data"));

  // clear bond store data
  bleHIDPeripheral.clearBondStoreData();

  bleHIDPeripheral.setReportIdOffset(1);
  bleHIDPeripheral.setLocalName("Arduino Core Mouse");

  bleHIDPeripheral.addHID(bleMouse);
  bleHIDPeripheral.addHID(bleSystemControl);

  bleHIDPeripheral.begin();

  Serial.println(F("BLE HID Demo"));

  coresensors.begin();
  // enable single tap detection
  coresensors.enableSingleTapDetection();
  // set up interrupt pin to sensor
  attachInterrupt(INT1, Tap, RISING);
}

void loop() {
  BLECentral central = bleHIDPeripheral.central();
  digitalWrite(LED_BUILTIN, LOW);
  if (central) {
    // central connected to peripheral
    Serial.print(F("Connected to central: "));
    Serial.println(central.address());
    
    count = 0;  //status of the pointer -> even if unpressed/odd if pressed
    while (central.connected()) {
    digitalWrite(LED_BUILTIN, HIGH);
       // check interrupt flag
      if (tap) {
        tap = 0;
        uint8_t status = 0;
        // get single tap detection status
        coresensors.getStatusSingleTapDetection(&status);
        if (status)
        {
          // first time you tap
          if (count % 2 == 0){
          digitalWrite(12, HIGH); //turn on the led
          bleMouse.press();
          count++;
          }
          // second time you tap
          else {
          digitalWrite(12, LOW);  //turn off the led
          bleMouse.release();
          count ++;
          }
        }
      }
      
      int32_t accelerometers[3] = {0};  //clean accelerometers values
      coresensors.getAccelerometer(accelerometers);
      //do not get new values if: the board is facing downward, board is only slightly tilted, tap has just been detected.
      if (accelerometers[2] > 0 && (abs(accelerometers[0])>100||abs(accelerometers[1])>100) && (millis()>(crono+200))) {
        int mappedX = map (accelerometers[0], -1023, 1023, -12, 12);
        int mappedY = map (accelerometers[1], -1023, 1023, -12, 12);
        int x = -1 * mappedX;
        int y = mappedY;
        bleMouse.move(x, y);
      }
    }
    // central disconnected
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    //built-in pin off when disconnected
    digitalWrite(LED_BUILTIN, LOW);
  }
}

// interrupt service routine
void Tap()
{
  tap = 1;
  //update time
  crono = millis();
}
