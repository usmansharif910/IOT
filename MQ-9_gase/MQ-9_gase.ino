void setup() 
{
  Serial.begin(9600);
}
 
void loop() 
{
  float sensorVoltage; 
  float sensorValue;
 
  sensorValue = analogRead(A0);
  sensorVoltage = sensorValue/1024*3.3;
 
//  Serial.print("sensor voltage = ");
//  Serial.print(sensorVoltage);
//  Serial.println(" V");
  Serial.print("sensor value = ");
  Serial.print(sensorValue);
  Serial.println(" CO");
  delay(1000);
}
