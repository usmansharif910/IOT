#include <ESP8266WiFi.h>                 
//#include <FirebaseESP8266.h>   
#include <FirebaseArduino.h>
   
#include <DHT.h>              
 
#define FIREBASE_HOST "sensortestingdht-default-rtdb.firebaseio.com"      
#define FIREBASE_AUTH "ka7IwUEFCk35HEmaoL7C9xYXmh434JS218YsWCYp"            
#define WIFI_SSID "Twenty Twenty-Two"                                  
#define WIFI_PASSWORD "usman#2211"            
 
#define DHTPIN 2                                            // Digital pin connected to DHT11
#define DHTTYPE DHT22                                        // Initialize dht type as DHT 11
DHT dht(DHTPIN, DHTTYPE);                                                    
 
void setup() 
{
  Serial.begin(9600);
  dht.begin();                                                  //reads dht sensor data 
               
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                  
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("connecting");
    delay(500);
  }
 
//  Serial.println();
  Serial.print("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());                               //prints local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  // connect to the firebase
//  if (Firebase.failed()) 
//    {
// 
//      Serial.print("firebase connection error");
//      Serial.println(Firebase.error()); 
//      return;
//  }
  
}
 
void loop() 
{
  float h = dht.readHumidity();                                 // Read Humidity
  float t = dht.readTemperature();                              // Read temperature
  
  if (isnan(h) || isnan(t))                                     // Checking sensor working
  {                                   
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  } 
  Serial.print("Humidity: ");  
  Serial.print(h);
  String fireHumid = String(h) + String("%");                   //Humidity integer to string conversion
  
  Serial.print("%  Temperature: ");  
  Serial.print(t);  
  Serial.println("°C ");
  String fireTemp = String(t) + String("°C");                  //Temperature integer to string conversion
  delay(5000);
 
  Firebase.setString("Temperature", fireTemp); 
  Firebase.setString("Humidity", fireHumid);  //setup path to send Humidity readings
          //setup path to send Temperature readings
  Firebase.pushString("Temperature", fireTemp);
  Firebase.pushString("Humidity", fireHumid);
  
    if (Firebase.failed()) 
    {
 
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error()); 
      return;
  }
}
