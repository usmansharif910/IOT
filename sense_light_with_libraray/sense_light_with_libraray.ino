#include <Wire.h>
#include <Digital_Light_TSL2561.h>
int lux;
void setup()
{
Wire.begin();
Serial.begin(9600);
TSL2561.init();
}

void loop()
{
Serial.print("The Light value is: ");
lux=TSL2561.readVisibleLux();
Serial.print(lux);
delay(1000);
}

//This code works fine, but now I would like to read out the
//infrared-signal.

//I have also found the other libary:

//#include <Wire.h>
//#include "Digital_Light_ISL29035.h";
//
//void setup() {
//Wire.begin();
//Serial.begin(9600);
//
//if(ISL29035.init() < 0)
//Serial.println("ISL29035 init failed!");
//}
//
//void loop() {
//Serial.println("----");
//Serial.print("Ambient light intensity: ");
//Serial.println(ISL29035.readVisibleLux());
//
//Serial.print("IR intensity: ");
//Serial.println(ISL29035.readIRLux());
//
//Serial.print("Exposure value: ");
//Serial.println(ISL29035.readEV());
//
//delay(1000);
//}
