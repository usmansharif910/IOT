#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
const char *ssid = "Twenty Twenty-Two";  //ENTER YOUR WIFI ssid
const char *password = "usman#2211";  //ENTER YOUR WIFI password


void setup() {
connectWifi();
}


void loop() {
SendSensorData();
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
  String sensorData1,sensorData2,sensorData3,sensorData4,sensorData5,sensorData6,sensorData7, postData;
  sensorData1="High";
    sensorData2="High";
//      sensorData3="High";
//        sensorData4="High";
//          sensorData5="High";
//            sensorData6="High";
//              sensorData7="High";
  //Post Data
  postData = "sensor1=" +  sensorData1 + "&sensor2=" + sensorData2;
  
  http.begin("http://10.109.64.11/esp8266/postData.php");              //change the ip to your computer ip address
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
 
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
 
  http.end();  //Close connection
  
  delay(5000);  //Post Data at every 5 seconds
}
