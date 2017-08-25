
/*
  DigitalPinBoard.ino

  Written by Federico Sanna
  
  This example shows how to use your Arduino Primo as a digital 
  PinBoard. Using a smarthphone and the Nordic nRF Connect App
  you can store a message in the Arduino, that can be read by
  any device with NFC enabled. The message will stay in the memory
  until it will be overwritten. You will be able to read the message 
  stored in the board at any time by passing a device with NFC 
  enabled over your board.
  The idea is to create a rewritable and digital post-it that goes 
  paperless and inkless (and is much coller to have in the entrance
  of the house).

  Up to now this sketch allows you to store a maximum of 20 characters
  in the board due to size limit in the BLE communication as set
  in the Nordic App. The sketch can easily be implemented to increase
  this limit by receiving and storing multiple segments of 20 characthers
  messages. 
  
 */

#include <NFC.h>
#include <BLEPeripheral.h>
#include <BLESerial.h>

// create ble serial instance
BLESerial bleSerial = BLESerial();

//Message that will be displayed until another one will be written over it
char Message[ ] = "Here you can store a message on the board that other users can read with their NFC. Max number of characters you can store: 20";

void setup() {
  // custom services and characteristics can be added as well
  bleSerial.setLocalName("Primo Pin Board");

  NFC.setTXTmessage(Message, "en");
  NFC.start();

  //initialize BLE led
  pinMode(BLE_LED, OUTPUT);

  Serial.begin(9600);
  bleSerial.begin();
}

void loop() {
  bleSerial.poll();

  if (bleSerial && Serial) {

    int byte;
    int n = 0;
    if ((byte = bleSerial.read()) > 0) {  //message from bluetooth is read
      while (byte > 0) {
        Serial.write((char)byte);   //the message is stored in the board
        Message[n] = (char)byte;
        n++;
        //byte = bleSerial.read();
      }
        for ( ; n < sizeof(Message); n++) {
          Message[n] = ' ';
        }
        Message [n + 1] = '\0';
      NFC.stop();
      NFC.setTXTmessage(Message, "en");
      NFC.start();
    }
  }

  if (bleSerial.status() == ADVERTISING) {
    digitalWrite(BLE_LED, LOW);
    delay(200); \
    digitalWrite(BLE_LED, HIGH);
    delay(200);
  }
  else {    // if we are not advertising, we are connected
    digitalWrite(BLE_LED, HIGH);
  }
}

