#include <ESP8266WiFi.h>
#include <WiFiClient.h>

WiFiClient client;
const IPAddress server(192,168,8,102);

const int httpPort = 80;


const char* ssid = "Twenty Twenty-Two";
const char* password =  "usman#2211";
void setup()
{
  Serial.begin(9600);
  
  Serial.println();
  Serial.println("Booted");
  Serial.println("Connecting to Wi-Fi");

  WiFi.begin (ssid, password);
//  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    yield();
  }
  Serial.println("WiFi connected");

 if (client.connect(server,httpPort )){
 Serial.println("Client Connected");

 }
 
 else{
     Serial.println("No Connection");
 }


}
