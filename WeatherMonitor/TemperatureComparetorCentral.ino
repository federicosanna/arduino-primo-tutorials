/*
  TemperatureComparatorCentral.ino
  
  Modified by Federico Sanna
  
  This code is to be uploaded  on a Primo Core Board. It will send a byte via BLE to the the Primo board,
  which will consequentially search on internet the current temperature in a city in the world and will 
  send this information to the Primo Core.
  For this example the Peripheral should be an Arduino Primo, with the file GetTemperatureOnlinePeripheral.ino uploaded.

  Once received, the temperature of the city (acquired via BLE) and the temperature in the room (aquired
  via sensors), can be read by bringing close to the Primo Core a smartphone (or another device), with NFC
  reader anabled.

  In this example BLE_LED shows the status of the board. It will blink every 200 ms when the board is advertising.
  It will be on when the board is connected to a central. It will be off when the board is disconnected.

*/

#include <CoreSensors.h>
#include <BLECentralRole.h>
#include <BLESerial.h>
#include <NFC.h>

// create central instance
BLECentralRole                   bleCentral                       = BLECentralRole();

// create remote service with UUID compliant to NUS service
BLERemoteService                 remoteService                    = BLERemoteService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");

// create remote characteristics with UUID and properties compliant NUS service
BLERemoteCharacteristic          remoteRxCharacteristic           = BLERemoteCharacteristic("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWriteWithoutResponse);
BLERemoteCharacteristic          remoteTxCharacteristic           = BLERemoteCharacteristic("6E400003-B5A3-F393-E0A9-E50E24DCCA9E", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  
  // add service and characteristics
  bleCentral.addRemoteAttribute(remoteService);
  bleCentral.addRemoteAttribute(remoteTxCharacteristic);
  bleCentral.addRemoteAttribute(remoteRxCharacteristic);

  // assign event handlers for central events
  bleCentral.setEventHandler(BLEScanReceived, receiveAdvPck);  
  bleCentral.setEventHandler(BLEConnected, bleCentralConnectHandler);
  bleCentral.setEventHandler(BLEDisconnected, bleCentralDisconnectHandler);
  bleCentral.setEventHandler(BLERemoteServicesDiscovered, bleCentralRemoteServicesDiscoveredHandler);

  // assign event handlers for characteristic
  remoteTxCharacteristic.setEventHandler(BLEValueUpdated, bleRemoteTxCharacteristicValueUpdatedHandle);
 
  // set scan parameters
  // interval and window in 0.625 ms increments 
  bleCentral.setScanInterval(3200); // 2 sec
  bleCentral.setScanWindow(800);  // 0.5 sec

  // begin initialization and start scanning
  bleCentral.begin(); 

  // start the core sensors
  coresensors.begin();
  
  Serial.println("Scanning...");
}

void loop() {
 // if a device is connected send data read from the Serial port
 if(bleCentral.connected()){
  delay(30000);
  unsigned char trigger = 1;
         bleCentral.writeRemoteCharacteristic(remoteRxCharacteristic, &trigger, sizeof(trigger));
     
  // turn BLE led on
  digitalWrite(LED_BUILTIN, HIGH);
  int len = 0;
  }
  else // if we are not connected we are scan. Move the BLE led accordingly
	blinkOnScan();
}

void blinkOnScan(){
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
}

void receiveAdvPck(BLEPeripheralPeer& peer){
  char scannedUuid[31];
  byte len;
  // search for the NUS's uuid
  peer.getFieldInAdvPck(BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE, scannedUuid, len);
  if(len == 0) // field not found
    peer.getFieldInAdvPck(BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE, scannedUuid, len);
  if(len != 0){ // the field was found
    if(!strcmp(scannedUuid, remoteService.rawUuid()))
      // the uuids match. Connect to the peripheral
      bleCentral.connect(peer);
   } 
}

void bleCentralConnectHandler(BLEPeripheralPeer& peer) {
  // peer connected event handler
  Serial.print("Connected event, peripheral: ");
  Serial.println(peer.address());
}

void bleCentralDisconnectHandler(BLEPeripheralPeer& peer) {
  // peer disconnected event handler
  Serial.print("Disconnected event, peripheral: ");
  Serial.println(peer.address());
}

void bleCentralRemoteServicesDiscoveredHandler(BLEPeripheralPeer& peer) {
  // peer remote services discovered event handler
  Serial.print("Remote services discovered event, peer: ");
  Serial.println(peer.address());

  // subscribe Tx characteristic in order to automatically receive
  // messages sent from peripheral
  if (remoteTxCharacteristic.canSubscribe()) {
    remoteTxCharacteristic.subscribe();
  }
}


void bleRemoteTxCharacteristicValueUpdatedHandle(BLEPeripheralPeer& peer, BLERemoteCharacteristic& characteristic) {
  // print the incoming message
  NFC.stop();   // notice that NFC.stop() was placed here to leave some time before the message is updated
  digitalWrite(LED_BUILTIN, HIGH);
  
  char * message = (char *)remoteTxCharacteristic.value();
  for(int i=0; i<remoteTxCharacteristic.valueLength(); i++)
  Serial.print(message[i]);
  Serial.println();
  
  float temperature;
  temperature = coresensors.getTemperature();
  String thisString = String(temperature,2);
  
  char toprint[] = { 'T', 'e', 'm', 'p', 'e', 'r', 'a', 't', 'u', 'r', 'e', ' ', 'o', 'u', 't', 's', 'i', 'd', 'e', ':', 
                      message[0], message[1], message[2],'\n', 
                      'T', 'e', 'm', 'p', 'e', 'r', 'a', 't', 'u', 'r', 'e', ' ', 'i', 'n', 's', 'i', 'd', 'e', ':',  
                      thisString[0],thisString[1],thisString[2],thisString[3], '\0' }; 
  NFC.setTXTmessage(toprint, "en");
  // start the NFC module
  NFC.start();
  digitalWrite(LED_BUILTIN, LOW);

 }
