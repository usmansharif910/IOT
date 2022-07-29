/* LDR Luximeter */
// Constants
#define VIN 3.3 // V power voltage, 3.3v in case of NodeMCU
#define R 10000 // Voltage devider resistor value

// Parameters
const int Analog_Pin = 0; // Analog pin A0

//Variables
int LDR_Val; // Analog value from the LDR
int Iluminance; //Lux value

void setup(void) {
  
  Serial.begin(9600);
}

void loop(void) { 
  LDR_Val = analogRead(Analog_Pin);
  Iluminance = conversion(LDR_Val);
  Serial.print("Analog reading from sensor = "); 
  Serial.println(LDR_Val); // between 0 - 
  Serial.print("Iluminance value = ");
  Serial.print(Iluminance); // Converted value
  Serial.println(" Lux"); 
  delay(500);


int conversion(int raw_val)
  // Conversion rule
  float Vout = float(raw_val) * (VIN / float(1023));// Conversion analog to voltage
  float RLDR = (R * (VIN - Vout))/Vout; // Conversion voltage to resistance
  int lux = 500/(RLDR/1000); // Conversion resitance to lumen
  return lux;
}
