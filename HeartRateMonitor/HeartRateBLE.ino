/*
  HeartRateBLE.ino

  Written by Federico Sanna

  This example demonstrates the use of the Serial Plotter (Tools->Serial Plotter) to display the heart beats and
  how to send to a smarthphone via BLE information on the heart rate according to bluetooth standards.
  To diplay the heart rate on the serial plotter the board has to be connected via bluetooth first.
  You can adjust the threshold value depending on who you are recording the value on to better capture the beats,
  and you can use the Nordic Toolbox App to receive the heart rate on your phone.
  The heart rate is calculated on values recorded every 10 milliseconds over an interval of 3 seconds, meaning that 
  the heart rate will be updated every 3 seconds.

  In this example BLE_LED shows the status of the board. It will blink every 200 ms when the board is scanning.
  It will be on when the board is connected to a peripheral. It will be off when the board is disconnected.

  This example code is in the public domain.

*/

#include <BLEPeripheral.h>

#define SIGNAL A0

BLEPeripheral blePeripheral;          // BLE Peripheral Device
BLEService heartRateService("180D");  // BLE Heart Rate Service

// BLE Heart Rate Measurement Characteristic"
BLECharacteristic heartRateChar("2A37",  // standard 16-bit characteristic UUID
                                BLERead | BLENotify, 2);  // remote clients will be able to get notifications if this characteristic changes
// the characteristic is 2 bytes long as the first field needs to be "Flags" as per BLE specifications
// https:/developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.heart_rate_measurement.xml

void findHeartRate (float averageSample);   //function to process the array of samples to calculate and print the heart rate

int sample[300] = { 0 };  //create an array with 300 samples, each taken every 10 milliseconds over a period of 3 seconds
int n = 0;                //use to navigate between the elements of the array when storing them

int sumSample = 0;        //sum of all the values in the array. Used to find an average value
float averageSample = 0;  //average of all the 300 values in the array

void setup() {
  Serial.begin(250000);

  blePeripheral.setLocalName("HeartRateSketch");                     // Set a local name for the BLE device
  blePeripheral.setAdvertisedServiceUuid(heartRateService.uuid());   // add the service UUID
  blePeripheral.addAttribute(heartRateService);                      // Add the BLE Heart Rate service
  blePeripheral.addAttribute(heartRateChar);                         // add the Heart Rate Measurement characteristic

  blePeripheral.begin();  //activate BLE device to continuosly transmit BLE advertising packets.
  //Your board will be visible to central devices until it receives a new connection
  Serial.println("Bluetooth device active, waiting for connections...");

}


void loop() {

  BLECentral central = blePeripheral.central();   // listen for BLE peripherals to connect

  if (central) {                             //when connected to a central
    Serial.print("Connected to central: ");
    Serial.println(central.address());       // print the central's MAC address:
    //turn on BLE led
    digitalWrite(BLE_LED, HIGH);

    // as long as the central is still connected:
    while (central.connected()) {
      //every 10 millisecs enter here. In this way we ensure that we will have samples that are equaly time spaced, each of them at 10ms from one another.
      if (millis() % 10 == 0) {
        int rawValue = analogRead(A0);
        int sensorValue = map(rawValue, 0, 1023, 0, 255);
        Serial.println(sensorValue);  //use the Serial Plotter found in the Tools to visualise the recordings stored in your array.

        sumSample = sumSample + sensorValue;  //we update the total sum at every value recorded
        sumSample = sumSample - sample[n];

        sample[n] = sensorValue;              //writing over the old array the new values
        n++;
        averageSample = (float)sumSample / 300; //average over the 300 samples

        //once we recorded values over 3 second and we have a completely new array we calculate the heart rate based on it
        if (n == 300) {
          n = 0;
          findHeartRate(averageSample);   //function to calculate and print the Hear Rate
        }
        delay(1); //the delay ensures that we enter the loop only once every 10 ms.
        //Without the delay we might enter more than once if the program runs in less than 1 ms.
      }
    }
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
  digitalWrite(BLE_LED, LOW);
  delay(200);
  digitalWrite(BLE_LED, HIGH);
  delay(200);
}

void findHeartRate (float averageSample) {  //function to calculate and print the Hear Rate
  /*  We are interested in finding the time between pulses, from which we can find the Heart Rate.
      Since we made sure our samples are exactly at 10ms difference, we only need to know how many samples we have between two beats.
      In calculating the Heart Rate we have to remember that between 3 peaks, for example, we have only 2 intervals of time. */
  int count = 0;      //will use this to start counting the time after the first beat and discard everything before it.
  int totalTime = 0;  //counts the sum of the time of every complete cycle over the period of 3 seconds
  int lastI = 0;      //time at which the last beat occured (based on its position on the array)-->for example the time between two samples with 9 samples in between is 100ms
  bool trendState = false;    //check if we cross the threshold value
  bool goOverThreshold = false; //goes true when we are above the threshold
  int heartRate = 0;  //heart rate in beats per minute
  float setThreshold = 1.25;  //by changing this value you can modify the threshold over which a beat is detected.


  for (int i = 0; i < 300; i++) {
    goOverThreshold = (sample[i] > (averageSample * setThreshold));   //the threshold is adjusted at 125% the average, we consider that a beat is occuring when we pass this threshold
    if (goOverThreshold != trendState) {              //recognise when we are passing the threshold, either from high to low or viceversa
      trendState = goOverThreshold;
      if (goOverThreshold == false) {                 //star counting the end of the peak
        //when you find the first beat set count to 1 and start measuring the time
        if (count > 0) {
          totalTime = totalTime + (i - lastI);  //we will increment the time every time that we detect a full cycle. Time is counted using the position of the sample in the array
        }
        count++;
        lastI = i;
      }
    }
  }
  //HeartRate = (number of cardiac cycles) * (60seconds) / (time in seconds)  =  (number of beats - 1) * (60seconds) / (time in milliseconds * 0.001)
  heartRate = 6000 * (count - 1) / totalTime; //the heart rate is calculated by considering the total time for the beats recorded over the 3 seconds
  const unsigned char heartRateCharArray[2] = { 0, (char)heartRate };
  heartRateChar.setValue(heartRateCharArray, 2);  //update the heart rate measurement characteristic

  //Serial.print("heartRate is: "); //UNCOMMENT to send heart rate measure to Serial Monitor
  //Serial.println(heartRate);      //UNCOMMENT to send heart rate measure to Serial Monitor
}
