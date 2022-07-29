int noise, noiseValue;

void setup() {
Serial.begin(9600); // setup serial
}
void loop() {
  noise= analogRead(A0);
  noiseValue =map(noise,0,1024,50,140);
  Serial.print(noiseValue);
  Serial.print("\n");
  
delay(200);
}

// const int OUT_PIN = 8;
// 
// void setup() {
//   Serial.begin(9600);
// }
// 
// void loop() {
//   Serial.println(digitalRead(OUT_PIN));;
//   
//   delay(50);
//}


//const int OUT_PIN = 8;
// const int SAMPLE_TIME = 10;
// unsigned long millisCurrent;
// unsigned long millisLast = 0;
// unsigned long millisElapsed = 0;
// int sampleBufferValue = 0;
//
// void setup()
// { 
//   Serial.begin(115200);
// }
//
//
// void loop() {
// 
//   millisCurrent = millis();
//   millisElapsed = millisCurrent - millisLast;
// 
//   if (digitalRead(OUT_PIN) == LOW) {
//     sampleBufferValue++;
//   }
// 
//   if (millisElapsed > SAMPLE_TIME) {
//     Serial.println(sampleBufferValue);
//     sampleBufferValue = 0;
//     millisLast = millisCurrent;
//   }
//
//   
// 
// }
