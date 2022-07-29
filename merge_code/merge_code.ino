// Temp code
#include "DHT.h"
#include <ESP8266WiFi.h>

   String apiKey = "REZ14N37QESTP8SY"; //Write API key of your ThingSpeak channel
   const char* server = "api.thingspeak.com"; // API for thingspeak
   float h;
   float t;
   int light;
   const char *ssid = "Twenty Twenty-Two"; // Wifi SSID of your Internet connection
   const char *pass = "usman#2211"; // Password
#define DHTPIN D4 //pin where the DHT22 is connected
   DHT dht(DHTPIN, DHT22);
   WiFiClient client;

//*Temp code

//Gps Code
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

   static const int RXPin = 4, TXPin = 3;// Here we make pin 4 as RX of arduino & pin 3 as TX of arduino 
   static const uint32_t GPSBaud = 9600;
   TinyGPSPlus gps;
   SoftwareSerial ss(RXPin, TXPin);

// *GPS Code

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud); //GPS line
 // delay(10);
  pinMode(D1, OUTPUT);
  pinMode(D3, OUTPUT);
  digitalWrite(D1,HIGH);
  digitalWrite(D3,LOW);
  //delay(1000);
  dht.begin();
  Serial.println("Trying to Connect with ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass); // Connecting ESP8266 with Internet enabled Wifi with above mentioned credentials
while (WiFi.status() != WL_CONNECTED)
{
  // If the connection was unsuccesfull, it will try again and again
  //delay(500);
  Serial.print(".");
}
  // Connection succesfull
  Serial.println("");
  Serial.println("WiFi connected");
}
void loop()
{
//GPS
while (ss.available() > 0)
  if (gps.encode(ss.read()))
  displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
   Serial.println(F("No GPS detected: check wiring."));
   //while(true);
  }
  //*GPS
if (isnan(h) || isnan(t))
{
   Serial.println("Failed to read from DHT sensor!");
   return;
}
  // Connecting to the Thingspeak API and Posting DATA
if (client.connect(server,80)) // "184.106.153.149" or api.thingspeak.com
{
// Format of DATA Packet "Write API Key&amp;field1=Temperature data&amp;field2=Humidity Data"
   desplaytemp();
   lightsensor();  
}
  client.stop();
  Serial.println("\n\n Waiting to initiate next data packet...");
// thingspeak needs minimum 15 sec delay between updates
delay(3000);
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
}
 void desplaytemp() 
 {
  float h = dht.readHumidity(); // Reading Temperature form DHT sensor
  float t = dht.readTemperature();

// Format of DATA Packet "Write API Key&amp;field1=Temperature data&amp;field2=Humidity Data"
  String postStr = apiKey;
  postStr +="&field1=";
  postStr += String(t);
  postStr +="&field2=";
  postStr += String(h);
  postStr += "\r\n\r\n";
  client.print("POST /update HTTP/1.1\n");
  client.print("Host: api.thingspeak.com\n");
  client.print("Connection: close\n");
  client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
  client.print("Content-Type: application/x-www-form-urlencoded\n");
  client.print("Content-Length: ");
  client.print(postStr.length());
  client.print("\n\n");
  client.print(postStr);
  client.flush();
  //Serial.println(postStr);
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celcius, Humidity: ");
  Serial.print(h);
  Serial.println("%.");
  Serial.println("Data has been sussecfully sent to Thingspeak.");
  //Serial.print(t);
  //Serial.print(h);
  int analogValue;
  float temp;
  Serial.print("Temperature");
  Serial.print(t);
  Serial.print("\n");
  Serial.print("Humidity");
  Serial.print(h);
  }


  void lightsensor(){

    light = analogRead(A0);
    Serial.print("LDR value\n");
    Serial.println(light);
    }
