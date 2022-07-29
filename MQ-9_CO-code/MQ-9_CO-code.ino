 /* 
MQ9 Calibration 
modified on 19 Feb 2019 
by Saeed Hosseini 
https://electropeak.com/learn/ 
*/ 
void setup() { 
 Serial.begin(9600); 
} 
void loop() { 
 float sensor_volt;  
 float RS_air; //  Rs in clean air 
 float R0;  // R0 in 1000 ppm LPG 
 float sensorValue; 
 float sensor;
 float co;
//Average   
   for(int x = 0 ; x < 100 ; x++) 
 { 
   sensorValue = sensorValue + analogRead(A0); 
   sensor =analogRead(A0);
   co = map(sensor, 0, 1024, 0, 100);
 } 
 sensorValue = sensorValue/100.0; 
//-----------------------------------------------/ 
 sensor_volt = (sensorValue/1024)*5.0; 
 RS_air = (5.0-sensor_volt)/sensor_volt; // Depend on RL on yor module 
 R0 = RS_air/9.9; // According to MQ9 datasheet table 
// Serial.print("sensor_volt = "); 
 //Serial.print(sensor_volt); 
 Serial.print("Corbon Mono : "); 
 Serial.print(co);
 Serial.println("\n\n");
 //Serial.print("R0 = "); 
 //Serial.println(R0); 
 delay(2000); 
}
