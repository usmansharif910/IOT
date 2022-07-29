#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

 #define DHTPIN 2 //pin where the DHT22 is connected
  DHT dht(DHTPIN, DHT22);

  #include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3;// Here we make pin 4 as RX of arduino & pin 3 as TX of arduino 
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);
  
const char *ssid = "Twenty Twenty-Two";  //ENTER YOUR WIFI ssid
const char *password = "usman#2211";  //ENTER YOUR WIFI password



  float t;
  float h;
  float sensorVoltage; 
  float sensorValue;
  int soundvalue;
   float decible;
   int LDRpin = A0; //LDR Pin Connected at A0 Pin
   int val;


void setup() {
//   ss.begin(GPSBaud);
connectWifi();
dht.begin();

 pinMode(D1, OUTPUT);
  pinMode(D3, OUTPUT);
  digitalWrite(D1,HIGH);
  digitalWrite(D3,LOW);
}


void loop() {
  temprature();
  corbonmono();
  SendSensorData();
  noise();
  light();
  GPS();
   displayInfo();

  
}
//function to connect to wifi
void connectWifi(){
  delay(1000);
  Serial.begin(9600);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //If connection successful show IP address in serial monitor 
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}


//function to send sensor data 
void SendSensorData() {
  
  HTTPClient http;    //Declare object of class HTTPClient
  String temp,humidity,corbonmono,sensorData4,sensorData5,sensorData6,sensorData7, postData,noise,light,deviceNumber;
  deviceNumber=1;
  temp=t;
    humidity=h;
      corbonmono=sensorValue;
        noise=decible;
          light=val;
//            sensorData6="High";
//              sensorData7="High";
  //Post Data
  postData = "temp=" +  temp + "&humidity=" + humidity + "&corbonmono=" + corbonmono + "&sound=" + noise + "&light=" + light + "&device=" + deviceNumber;
  
  http.begin("http://192.168.8.101/esp8266/postData.php");              //change the ip to your computer ip address
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(deviceNumber);
  Serial.println(sensorValue);
 Serial.println(t);
  Serial.println(h);
    Serial.println(decible);
    Serial.println(val);
  
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
 
  http.end();  //Close connection
  
  delay(1000);  //Post Data at every 5 seconds
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

 void noise(){
 //  Serial.println(analogRead(A0));
     soundvalue = (analogRead(A0));
     decible = map(soundvalue,20,900,49.5,90);  
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
  delay(5000);
}
