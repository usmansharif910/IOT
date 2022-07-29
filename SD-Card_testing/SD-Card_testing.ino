#include <SPI.h>
#include <SD.h>
//#include "DHT.h" // library for getting data from DHT
//#include <DHT.h>
#include "DHT.h"

 #define DHTPIN 2  

 #define DHTTYPE DHT22 

 DHT dht(DHTPIN, DHTTYPE);

File myFile;
void setup() {
// Open serial communications and wait for port to open:
Serial.begin(9600);

 
while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}
Serial.print("Initializing SD card...");
//if (!SD.begin(10)) {
//Serial.println("initialization failed!");
//while (1);
//}
Serial.println("initialization done.");
// open the file. note that only one file can be open at a time,
// so you have to close this one before opening another.
myFile = SD.open("test.txt", FILE_WRITE);
// if the file opened okay, write to it:
if (myFile) {
Serial.print("Writing to test.txt...");
myFile.println("This is a test file :)");
myFile.println("testing 1, 2, 3.");
for (int i = 0; i < 20; i++) {
myFile.println(i);
}
// close the file:
myFile.close();
Serial.println("done.");
} else {
// if the file didn't open, print an error:
Serial.println("error opening test.txt");
}
// open the file for reading:
myFile = SD.open("test.txt");
if (myFile) {
Serial.println("test.txt:");
// read from the file until there's nothing else in it:
while (myFile.available()) {
Serial.write(myFile.read());
}
// close the file:
myFile.close();
} else {
// if the file didn't open, print an error:
Serial.println("error opening test.txt");
}
}
void loop() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
    
    delay(3000);//Wait 5 seconds before accessing sensor again.
// nothing happens after setup
}
