#include "DHT.h"
#include <SPI.h>
#include <Ethernet.h>
//A4-34-D9-13-3A-9A
byte mac[] = { 0xA4, 0x34, 0xD9, 0x13, 0x3A, 0x9A }; //Setting MAC Address

#define DHTPIN 2

#define DHTTYPE DHT22

DHT dht(DHTPIN,DHTTYPE);


float humidityData;
float temperatureData;


char server[] = "192.168.8.102";
IPAddress ip(192,168,8,100); 
EthernetClient client; 

/* Setup for Ethernet and RFID */

void setup() {
  Serial.begin(9600);
  dht.begin();
  if (Ethernet.begin(mac) == 0) {
  Serial.println("Failed to configure Ethernet using DHCP");
  Ethernet.begin(mac, ip);
  }
  delay(500);
}
//------------------------------------------------------------------------------


/* Infinite Loop */
void loop(){
  humidityData = dht.readHumidity();
  temperatureData = dht.readTemperature();
//   Serial.println(humidityData);
//   Serial.println(temperatureData);
  Sending_To_phpmyadmindatabase(); 
//Serial.println("debuging");
  delay(300); // interval
}


  void Sending_To_phpmyadmindatabase()   //CONNECTING WITH MYSQL
 {
   if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    Serial.print("GET /DHT_TEST/dht.php?humidity=");
    client.print("GET /DHT_TEST/dht.php?humidity=");     //YOUR URL
    Serial.println(humidityData);
    client.print(humidityData);
    client.print("&temperature=");
    Serial.println("&temperature=");
    client.print(temperatureData);
    Serial.println(temperatureData);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: 192.168.8.101");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }
