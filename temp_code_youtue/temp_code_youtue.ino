#include "DHT.h"
#include <ESP8266WiFi.h>

String apiKey = "REZ14N37QESTP8SY"; //Write API key of your ThingSpeak channel
const char* server = "api.thingspeak.com"; // API for thingspeak

const char *ssid = "Twenty Twenty-Two"; // Wifi SSID of your Internet connection
const char *pass = "usman#2211"; // Password


#define DHTPIN 2 //pin where the DHT22 is connected
DHT dht(DHTPIN, DHT22);

WiFiClient client;

void setup()
{
  Serial.begin(115200);
  delay(10);
  pinMode(D1, OUTPUT);
  pinMode(D3, OUTPUT);
  digitalWrite(D1,HIGH);
  digitalWrite(D3,LOW);
  delay(1000);
  dht.begin();

  Serial.println("Trying to Connect with ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass); // Connecting ESP8266 with Internet enabled Wifi with above mentioned credentials
while (WiFi.status() != WL_CONNECTED)
{
  // If the connection was unsuccesfull, it will try again and again
  delay(500);
  Serial.print(".");
}
  // Connection succesfull
  Serial.println("");
  Serial.println("WiFi connected");

}

void loop()
{

 float h = dht.readHumidity(); // Reading Temperature form DHT sensor
float t = dht.readTemperature();

if (isnan(h) || isnan(t))
{
Serial.println("Failed to read from DHT sensor!");
return;
}
// Connecting to the Thingspeak API and Posting DATA
if (client.connect(server,80)) // "184.106.153.149" or api.thingspeak.com
{
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
Serial.println(postStr);
Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" degrees Celcius, Humidity: ");
Serial.print(h);
Serial.println("%.");
 
Serial.println("Data has been sussecfully sent to Thingspeak.");

 Serial.print(t);
  Serial.print(h);

  int analogValue;
  float temp;

   Serial.print("Temperature");
  Serial.print(t);
  Serial.print("\n");
   Serial.print("Humidity");
  Serial.print(h);
//  Serial.print(espval);
//   analogValue=analogRead(espval);
//   temp=float(analogValue)/1023;
//   temp=temp*500;
//   Serial.print("Temp:");
//   Serial.print(temp);
//   Serial.print("C");
   delay(1000);
}

client.stop();
Serial.println("Waiting to initiate next data packet...");
// thingspeak needs minimum 15 sec delay between updates
delay(2000);
}
