// Developed By M V Subrahmanyam
// Electrinics Innovation

#include "DHT.h" // library for getting data from DHT
//#include <ESP8266WiFi.h>
#include <ThingSpeak.h>;
 //Librarry connecting ESP8266 to connect with Wifi

// ***********************************************Thingspeak Credentials*****************
String apiKey = "REZ14N37QESTP8SY"; //Write API key of your ThingSpeak channel
const char* server = "api.thingspeak.com"; // API for thingspeak
//****************************************************************************************

const char *ssid = "Twenty Twenty-Two"; // Wifi SSID of your Internet connection
const char *pass = "usman#2211"; // Password


unsigned long myChannelNumber = 1751503; //Your Channel Number (Without Brackets)

const char * myWriteAPIKey = "REZ14N37QESTP8SY"; //Your Write API Key

int val;
int LDRpin = A0; //LDR Pin Connected at A0 Pin

#define DHTPIN 5 //pin where the DHT22 is connected
DHT dht(DHTPIN, DHT22);
//WiFiClient client;
void setup()
{








Serial.begin(115200);

delay(10);

// Connect to WiFi network

//WiFi.begin(ssid, pass);



ThingSpeak.begin(client);

  
Serial.begin(115200); // Serial monitor Baudrate
delay(10);
//******************PowerSupply to the Sensor**********************
pinMode(D1, OUTPUT);
pinMode(D3, OUTPUT);
digitalWrite(D1,HIGH);
digitalWrite(D3,LOW);
delay(1000);
//*********************************************************
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
float t = dht.readTemperature(); // Reading Humidity form DHT sensor
if (isnan(h) || isnan(t))
{
Serial.println("Failed to read from DHT sensor!");
return;
}
// Connecting to the Thingspeak API and Posting DATA
if (Client.connect(server,80)) // "184.106.153.149" or api.thingspeak.com
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



val = analogRead(LDRpin); //Read Analog values and Store in val variable

Serial.print(val); //Print on Serial Monitor

delay(100);

ThingSpeak.writeField(myChannelNumber, 3,val, myWriteAPIKey); //Update in ThingSpeak



delay(100);
}
//client.stop();

Serial.println("Waiting to initiate next data packet...");

// thingspeak needs minimum 15 sec delay between updates
delay(30000);











}
