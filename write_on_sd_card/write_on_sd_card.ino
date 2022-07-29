#include <SPI.h>
#include <SD.h>

// Example sketch for DHT22 humidity - temperature sensor
// Written by cactus.io, with thanks to Adafruit for bits of their library. public domain

#include "cactus_io_DHT22.h"
#define DHTTYPE DHT22
#define DHT22_PIN 8     // what pin on the arduino is the DHT22 data line connected to

// For details on how to hookup the DHT22 sensor to the Arduino then checkout this page
// http://cactus.io/hookups/sensors/temperature-humidity/dht22/hookup-arduino-to-dht22-temp-humidity-sensor

// Initialize DHT sensor for normal 16mhz Arduino.
DHT22 dht(DHT22_PIN);
// Note: If you are using a board with a faster processor than 16MHz then you need
// to declare an instance of the DHT22 using
// DHT22 dht(DHT22_DATA_PIN, 30);
// The additional parameter, in this case here is 30 is used to increase the number of
// cycles transitioning between bits on the data and clock lines. For the
// Arduino boards that run at 84MHz the value of 30 should be about right.



const int chipSelect=4;
File myFile;


  void setup() {
   
  Serial.begin(9600);
  pinMode(chipSelect, OUTPUT);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
      Serial.println("initialization failed!");
      return;
  }
  {
     Serial.println("initialization done.");
  }
/*  {
   if (SD.exists("data.txt")) {
    Serial.println("data.txt exists.");
  } else {
    Serial.println("data.txt doesn't exist.");
  }
  }*/
 
    myFile = SD.open("data.txt", FILE_WRITE);
    Serial.println("DHT22 Humidity - Temperature Sensor");
    Serial.println("RH\t\tTemp (C)\tTemp (F)\tHeat Index (C)\tHeat Index (F)");
  if (myFile) {
 
    myFile.print(dht.humidity); //Serial.print(" %\t\t");
    myFile.print(",");
    myFile.print(dht.temperature_C);// Serial.print(" *C\t");
    myFile.print(",");
    myFile.print(dht.temperature_F); //Serial.print(" *F\t");
    myFile.print(",");
    myFile.print(dht.computeHeatIndex_C()); //Serial.print(" *C\t");
    myFile.print(",");
    myFile.print(dht.computeHeatIndex_F()); //Serial.println(" *F");
    myFile.close();
  }
    else {
    Serial.println("Error opening file in setup.");
  }
  dht.begin();

  delay(1000);
}


void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  dht.readHumidity();
  dht.readTemperature();
 
  // Check if any reads failed and exit early (to try again).
  if (isnan(dht.humidity) || isnan(dht.temperature_C)) {
    Serial.println("DHT sensor read failure!");
    return;
  }
  Serial.print(dht.humidity); Serial.print(" %\t\t");
  Serial.print(dht.temperature_C); Serial.print(" *C\t");
  Serial.print(dht.temperature_F); Serial.print(" *F\t");
  Serial.print(dht.computeHeatIndex_C()); Serial.print(" *C\t");
  Serial.print(dht.computeHeatIndex_F()); Serial.println(" *F");
 
  // Wait a few seconds between measurements. The DHT22 should not be read at a higher frequency of
  // about once every 2 seconds. So we add a 3 second delay to cover this.
//  saveData();//save to sd card
// delay(3000);
   // make a string for assembling the data to log:


   myFile = SD.open("data.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    myFile.print(dht.humidity); //Serial.print(" %\t\t");
    myFile.print(",");
    myFile.print(dht.temperature_C);// Serial.print(" *C\t");
    myFile.print(",");
    myFile.print(dht.temperature_F); //Serial.print(" *F\t");
    myFile.print(",");
    myFile.print(dht.computeHeatIndex_C()); //Serial.print(" *C\t");
    myFile.print(",");
    myFile.print(dht.computeHeatIndex_F()); //Serial.println(" *F");
    myFile.close();
  }
  else {
   // if the file didn't open, print an error:
    Serial.println("error opening data.txt");
  }
   delay(3000);

}
