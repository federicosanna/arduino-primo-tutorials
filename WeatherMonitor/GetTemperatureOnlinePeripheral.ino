/*
   TemperatureSearcherPeripheral.ino
   
   Modified by Federico Sanna 

   This code is to be uploaded on a Primo Board which will search via wifi the temperature in a chosen
   city in the world (in this example is Cagliari, Italy) and will send it via BLE to a Master to which 
   the board should be connected.
   For this example the Central should be an Arduino Primo Core, with the file ________________  uploaded.

   To make a web request the boards need to be connected to a wifi network, whose name an passwork 
   shoud be included in the sketch. The info about the weather are obtained for the New York Times
   webpage. The reserch is triggered by an incoming signal from the Master.

   After the reserch, the temperature will be sent back to the Primo Core via BLE.

   In this example BLE_LED shows the status of the board. It will blink every 200 ms when the board is advertising.
   It will be on when the board is connected to a central. It will be off when the board is disconnected.

   

   To be uploaded on an Arduino Primo
   The Arduino Primo will serve as a BLE peripheral to an Arduino Primo Core, 
   which in this connection will be the central device.

   When an input is sent via BLE to the board, it will make a request to the
   server of the New Youk Times to obtain information about the weather in a
   particular city of the world, in the example Cagliari (Italy), is used
*/

#include <WiFiLink.h>
#include <BLEPeripheral.h>
#include <BLESerial.h>

// create ble serial instance
BLESerial bleSerial = BLESerial();

char ssid[] = "cclIT";      //  your network SSID (name)
char pass[] = "ht34!eG$";   // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;           // your network key Index number (needed only for WEP)
int count;                  // to search in the results from the server the informations we need
char memory[5];             // updated as information comes from the server and contains the last 3 characters received
char fahrenheit[4];         // two characters representing the temperature in Fahrenheit
char celsius[4];            // two characters representing the temperature in Celsius
bool flag = false;

int status = WL_IDLE_STATUS;
char server[] = "www.nytimes.com";    // name address for New York Times

// Initialize the WiFi client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
  // custom services and characteristics for BLE communication can be added 
  bleSerial.setLocalName("WeatherFinder");

  //initialize BLE led
  pinMode(BLE_LED, OUTPUT);

  Serial.begin(115200);
  bleSerial.begin();
  memory[4] = '\0';
  fahrenheit[3] = '\0';
  celsius[3] = '\0';
}

void loop() {

  bleSerial.poll();
  if (bleSerial && Serial) {    // if the communication Bluetooth is established
    int byte;
    if ((byte = bleSerial.read()) > 0) {  // and if receiving something 
      
      //Check if communication with wifi module has been established
      if (WiFi.status() == WL_NO_WIFI_MODULE_COMM) {
        Serial.println("Communication with WiFi module not established.");
      }

      // attempt to connect to Wifi network:
      while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
      }
      Serial.println("Connected to wifi");
      Serial.println("\nStarting connection to server...");
      // if you get a connection, report back via serial:
      if (client.connect(server, 80)) {
        Serial.println("connected to server");
        // Make a HTTP request (the following line can be modified to get info about another city):
        client.println("GET /gst/weather.html?detail=Cagliari--IY HTTP/1.1");
        client.println("Host: www.nytimes.com");
        client.println("Connection: close");
        client.println();
      }
      flag = true;    //if we went through this process, enter the next while loop
    }
  }

  while (flag) {
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available()) {
      char c = client.read();
      if (c == 176 && count < 2) //if you find the character '°', which in the ASCII table correspond to 176
      {
        if (count == 0) {       // store the first temperature read in fahrenheit
          fahrenheit[0] = memory[0];
          fahrenheit[1] = memory[1];
          fahrenheit[2] = memory[2];
        } else                  // store the second temperature read in celsius
        {
          celsius[0] = memory[0];
          celsius[1] = memory[1];
          celsius[2] = memory[2];
        }
        count++;  //after finding a temperature, increment count
      }

      memory[0] = memory[1];  //update memory at evey character coming from server
      memory[1] = memory[2];
      memory[2] = memory[3];
      memory[3] = c;

    }

    // if the server's disconnected, stop the client:
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting from server.");

      if (fahrenheit[1] == 62){   //we are expecting a ">" before the temperature in fahrenheit
        fahrenheit[1] = 32;       //and we must account for the fact that the temperature 
        fahrenheit[0] = 32;       //in fahrenheit can have from 1 to 3 digits (not expecting negative temperatures)
        } else if (fahrenheit[0] == 62){
        fahrenheit[0] = 32;
        }
       if (celsius[1] == 40){     //we are expecting a "(" before the temperature in celsius
        celsius[1] = 32;          //and we must account for the fact that the temperature
        celsius[0] = 32;          //in celsius can have from 1 to 3 digits (including minus sign)
        } else if (celsius[0] == 40){
        celsius[0] = 32;
        }
      Serial.print("Temperature in fahrenheit: ");
      Serial.print(fahrenheit);
      Serial.println("°");
      Serial.print("Temperature in celsius: ");
      Serial.print(celsius);
      Serial.println("°");
      
      client.stop();
      bleSerial.write(celsius);
      flag = false;
    }
  }

  // handle the BLE led. Blink when advertising
  if (bleSerial.status() == ADVERTISING) {
    digitalWrite(BLE_LED, LOW);
    delay(200);
    digitalWrite(BLE_LED, HIGH);
    delay(200);
  }
  else // if we are not advertising, we are connected
    digitalWrite(BLE_LED, HIGH);
}

