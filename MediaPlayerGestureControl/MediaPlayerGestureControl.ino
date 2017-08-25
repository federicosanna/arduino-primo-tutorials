/*  
  MediaPlayerGestureControl.ino

  Written by Federico Sanna
   
  This example shows hot to use the sensors on the Arduino Primo Core to play
  music on your phone or laptop. It uses the BLE library to send information 
  to the device and allows you to control the music remotely. If the board is 
  connected to a device, every time a command is detected the led will blink 
  and a comand will be sent to the device.
  Completely rotate the board downward by rotating it to the right to play or
  pause music. Tilt it to the right to play the next track, to the left to play
  the previous track. To increase/decrease the volume, tilt it forward/backward.
  
*/

#include <CoreSensors.h>
#include <BLEHIDPeripheral.h>
#include <BLEMultimedia.h>
#include <BLESystemControl.h>

// interrupt pin
#define INT1 21

// create ble peripheral instance
BLEHIDPeripheral bleHIDPeripheral = BLEHIDPeripheral();
BLEMultimedia bleMultimedia;
BLESystemControl bleSystemControl;

int orientation = 0;
void sendComand(void);
int crono = 0;  //keeps track of the time and help discard involontary movements and comands

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // start serial port at 115200 bps
  Serial.begin(115200);

  Serial.println(F("BLE HID Peripheral - clearing bond data"));
  // clear bond store data
  bleHIDPeripheral.clearBondStoreData();

  bleHIDPeripheral.setReportIdOffset(1);
  bleHIDPeripheral.setLocalName("Primo Player");

  bleHIDPeripheral.addHID(bleMultimedia);
  bleHIDPeripheral.addHID(bleSystemControl);

  bleHIDPeripheral.begin();

  // start the core sensors
  coresensors.begin();
  // enable enable 6D orientation
  coresensors.enable6DOrientation();
  // set up interrupt pin to sensor
  attachInterrupt(INT1, Orientation, RISING);
}

void loop() {
  BLECentral central = bleHIDPeripheral.central();

  if (central) {
    // central connected to peripheral
    Serial.print(F("Connected to central: "));
    Serial.println(central.address());

    while (central.connected()) {

      // check interrupt flag
      if (orientation)
      {
        orientation = 0;
        uint8_t status = 0;
        // get 6D orientation status
        coresensors.getStatus6DOrientation(&status);
        if (status)
        {
          // print 6D Orientation
          sendComand();

          digitalWrite(LED_BUILTIN, HIGH);
          delay(100);
          digitalWrite(LED_BUILTIN, LOW);
        }
      }
    }
  }
  // central disconnected
  Serial.print(F("Disconnected from central: "));
  Serial.println(central.address());
}

// interrupt service routine
void Orientation()
{
  orientation = 1;
}

void sendComand( void )
{
  uint8_t xl = 0;
  uint8_t xh = 0;
  uint8_t yl = 0;
  uint8_t yh = 0;
  uint8_t zl = 0;
  uint8_t zh = 0;

  coresensors.get6DOrientationXL(&xl);
  coresensors.get6DOrientationXH(&xh);
  coresensors.get6DOrientationYL(&yl);
  coresensors.get6DOrientationYH(&yh);
  coresensors.get6DOrientationZL(&zl);
  coresensors.get6DOrientationZH(&zh);

  //to send any comand you must start by having the board resting flat
  if (xl == 0 && yl == 0 && zl == 0 && xh == 0 && yh == 0 && zh == 1)
  {
    crono = millis();
    while (zh == 1) //stay here untill the orientation changes
      coresensors.get6DOrientationZH(&zh);  //keep recording the position
    if (millis() > crono + 100) { //only if you stayed flat for more than 100ms(discard involontary movements)
      coresensors.get6DOrientationXL(&xl);
      coresensors.get6DOrientationXH(&xh);
      coresensors.get6DOrientationYL(&yl);
      coresensors.get6DOrientationYH(&yh);
      coresensors.get6DOrientationZL(&zl);
      crono = millis(); //update clock
      if (yh == 1) //if pitching upward turn volume up
      {
        bleMultimedia.write(MMKEY_VOL_UP);
      }
      else if (yl == 1) //if pitching downward turn volume down
      {
        bleMultimedia.write(MMKEY_VOL_DOWN);
      } else if (xh == 1) //rolling to the left play previous song
      {
        bleMultimedia.write(MMKEY_SCAN_PREV_TRACK);
      } else if (xl == 1) //if you roll to the right it may mean 2 things
      {
        crono = millis();
        while (millis() < (crono + 300))  //if in the next 300ms it detects that you completely turned the board
        {
          coresensors.get6DOrientationZL(&zl);

          if (zl == 1) {
            bleMultimedia.write(MMKEY_PLAYPAUSE); //then play/pause the song
            break;
          }
        }
        if ( zl == 0 ) {
          bleMultimedia.write(MMKEY_SCAN_NEXT_TRACK); //if you just rolled to the right, play next song
        }
      }
    }
  }
}
