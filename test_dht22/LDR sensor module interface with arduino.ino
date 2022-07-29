int light;
void setup()
{
  Serial.begin(115200);
}

void loop()
{
//  unsigned int AnalogValue;
  light = analogRead(A0);
  Serial.println(light);
  delay(1000);  
}
