#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "DHT.h"
#include <ThingSpeak.h>


#include <TinyGPS++.h>
#include <SoftwareSerial.h>

  #define DHTPIN 2 //pin where the DHT22 is connected
  DHT dht(DHTPIN, DHT22);
  
  const char* ssid = "Twenty Twenty-Two"; //Your Network SSID
  const char* password = "usman#2211"; //Your Network Password
  
  int val;
  float t;
  float h;
  float db;
  float sensorVoltage; 
  float sensorValue;
  int LDRpin = A0; //LDR Pin Connected at A0 Pin
  float lev;
  int soundvalue;




static const int RXPin = 4, TXPin = 3;// Here we make pin 4 as RX of arduino & pin 3 as TX of arduino 
static const uint32_t GPSBaud = 9600;


TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);
  
  WiFiClient client;
  
  unsigned long myChannelNumber = 1751503; //Your Channel Number (Without Brackets)
  const char * myWriteAPIKey = "REZ14N37QESTP8SY"; //Your Write API Key
  
void setup()
{
  Serial.begin(9600);
  // ss.begin(GPSBaud);
  delay(10);
  pinMode(D1, OUTPUT);
  pinMode(D3, OUTPUT);
  digitalWrite(D1,HIGH);
  digitalWrite(D3,LOW);
  //delay(1000);
  dht.begin();
  // Connect to WiFi network
  
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);

}
  
void loop()
{
 
  temprature();
  corbonmono();
  noise();
  light();
  GPS();
 
  Serial.print("\n\nlight intensity: ");//Print on Serial Monitor
  Serial.print(val);//Print on Serial Monitor
  Serial.print(" lux \n");
  Serial.print("Temperature: ");
  Serial.print(t);//Print on Serial Monitor
  Serial.print(" °C \n");
  Serial.print("Humidity: ");
  Serial.print(h);//Print on Serial Monitor
  Serial.print("%\n");
  Serial.print("Corbon monooxide= ");
  Serial.print(sensorValue);
  Serial.println("CO");
 // Serial.print("\n");
//  Serial.println("Sound value= ");
//  Serial.print(soundvalue);
  Serial.print("\nsound: ");
  Serial.print(db);//Print on Serial Monitor
  Serial.print(" Db\n");
  displayInfo();

  
  // For Checking/ Debuging
//  int a=12;
//
//  int val=10;
//  int t=12;
//  int h=15;
  
//  delay(5000);
   upload();
   
  
}

void corbonmono(){
  
//  float reading = digitalRead(D0);
//  lev = ((reading/1023)*100);
//  if (isnan(lev))
//{
//  Serial.println("Failed to read from MQ-2 sensor!");
//  return;



 
  sensorVoltage = analogRead(A0);
    sensorValue = map(sensorVoltage, 0, 1024, 0, 100);
  //sensorValue = sensorVoltage/1024*3.3;
 
//  Serial.print("sensor voltage = ");
//  Serial.print(sensorVoltage);
//  Serial.println(" V");
  
  //delay(1000);
}

void temprature()
{
   h = dht.readHumidity(); // Reading Temperature form DHT sensor
   t = dht.readTemperature();
  
  if (isnan(h) || isnan(t))
  {
  Serial.println("Failed to read from DHT sensor!");
  return;
  }
  }


  void noise(){
 //  Serial.println(analogRead(A0));
     soundvalue = (analogRead(A0));
     db = map(soundvalue,20,900,49.5,90);  
//   sound =(soundValue+83.2073)/11.003;
}


  void light(){
     val = analogRead(LDRpin); //Read Analog values and Store in val variable
    }


   void GPS(){

       while (ss.available() > 0)
    if (gps.encode(ss.read()))
//     displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }

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

  //delay(100);
}

void upload(){
  ThingSpeak.writeField(myChannelNumber, 1,val, myWriteAPIKey); //Update in ThingSpeak
  ThingSpeak.writeField(myChannelNumber, 2,t, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 3,h, myWriteAPIKey); //Update in ThingSpeak
  ThingSpeak.writeField(myChannelNumber, 4,lev, myWriteAPIKey); //Update in ThingSpeak
  ThingSpeak.writeField(myChannelNumber, 7,db, myWriteAPIKey);
 delay(2000);
  
  }
