// ----------(c) Electronics-project-hub-------- //
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

#include <TinyGPS++.h>
#include <SoftwareSerial.h>



//------- WI-FI details ----------//
char ssid[] = "Twenty Twenty-Two"; //SSID here
char pass[] = "usman#2211"; // Passowrd here
//--------------------------------//

//----------- Channel details ----------------//
unsigned long Channel_ID = 1751503; // Your Channel ID
const char * myWriteAPIKey = "REZ14N37QESTP8SY"; //Your write API key
//-------------------------------------------//

static const int RXPin = 4, TXPin = 3;// Here we make pin 4 as RX of arduino & pin 3 as TX of arduino 
static const uint32_t GPSBaud = 9600;


TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

const int Field_Number_1 = 5;
const int Field_Number_2 = 6;
String value = "";
float lati = 0, longi = 0;
int x, y;
WiFiClient  client;

void setup()
{
  Serial.begin(115200);
   ss.begin(GPSBaud);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  internet();
}

void loop()
{

while (ss.available() > 0)
    if (gps.encode(ss.read()))
//     internet();
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }

  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }

  
 

  lati= (gps.location.lat());
  longi= (gps.location.lng());
//  if (Serial.available() > 0)
//  {
//    delay(100);
//    while (Serial.available() > 0)
//    {
//      value = Serial.readString();
//      if (value[0] == '*')
//      {
//        if (value[5] == '#')
//        {
//          value_1 = ((value[1] - 0x30) * 10 + (value[2] - 0x30));
//          value_2 = ((value[3] - 0x30) * 10 + (value[4] - 0x30));
//        }
//      }
//    }
//  }
  upload();
}

void internet()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      delay(50);
    }
  }
}

void upload()
{
  ThingSpeak.writeField(Channel_ID, Field_Number_1, lati, myWriteAPIKey);
//  delay(15000);
  ThingSpeak.writeField(Channel_ID, Field_Number_2, longi, myWriteAPIKey);
//  delay(100);
  value = "";

}




void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  
  Serial.println();

  delay(1000);
}
// ----------(c) Electronics-project-hub-------- //
