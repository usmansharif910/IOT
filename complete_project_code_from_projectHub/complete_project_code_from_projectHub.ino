/* RTC-Weather-Sensors_v6_MKR1000   by Zonker Harris   Spring 2016
 *  Hooray! A small board, with more than 2 hardware interrupts! 
 *  Plus WiFi, *and* crypto (VERY necessary for IoT applications!)
 *  
 * Note to newer hackers:  I put MANY comments in my code, as you see.
 *  When you click upload, *comments are IGNORED*, and do not eat memory!
 *  I recommend that you also add plenty of comments when you make changes
 *  and additions, to help you remember WHY you did what you did months ago.
 *  They will also help those who come after you, to learn a thing or two.
 *  
 * Now, libraries *DO* count against your program memory...
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_HTU21DF.h>

// Including neopixels as a placeholder, but the library interrupt vector needs updating.
// Include the Adafruit_NeoPixel library  https://github.com/adafruit/Adafruit_NeoPixel
//#include <Adafruit_NeoPixel.h>  
//const int numLeds = 1;  // How many neopixels in the string? used to set the NeoPixel library
  // Parameter 1 = number of pixels in strip
  // Parameter 2 = pin number (most are valid)
  // Parameter 3 = pixel type flags, add together as needed:
  // NEO_RGB Pixels are wired for RGB bitstream
  // NEO_GRB Pixels are wired for GRB bitstream
  // NEO_KHZ400 400 KHz bitstream (e.g. FLORA pixels)
  // NEO_KHZ800 800 KHz bitstream (e.g. High Density LED strip)
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLeds, 6, NEO_GRB + NEO_KHZ800);

/* The BMP085_U driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.
   (The BMP180 is comptible with this library, and gives the same output, but
    the library will identify the BMP180 as a BMP085.)
   
   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).  */

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10180);

/*  This sketch also designed to work with the HTU21D-F sensor from Adafruit
      ----> https://www.adafruit.com/products/1899  */

Adafruit_HTU21DF htu = Adafruit_HTU21DF();

/*   Macetech Chronodot v2.1 Battery-backed Real-Time Clock (RTC)...
     http://docs.macetech.com/doku.php/chronodot 
     (because the battery on my MKR1000 doesn't seem to keep the on-board RTC alive)

   The Adafruit Real-Time Clock (RTC) Library info
     https://learn.adafruit.com/adafruit-data-logger-shield/using-the-real-time-clock

   Analog Devices TMP36 analog calibrated temperature sensor.
     This requires some math  https://learn.adafruit.com/tmp36-temperature-sensor
     http://www.analog.com/media/en/technical-documentation/data-sheets/TMP35_36_37.pdf

   ML8511 UV sensor... This sensor detects 280-390nm light most effectively. 
     This is categorized as part of the UVB (burning rays) spectrum and most of 
     the UVA (tanning rays) spectrum.

   MOD-1016 (AS3935 Lightning Sensor)  i2c address 0x03 - AS3935 Lightning Sensor 

   Adafruit has an anemometer which provides a DC voltage out (0.4-2.0v)
     https://www.adafruit.com/products/1733
     http://www.instructables.com/id/Breezefinder-Citizen-Science-Windpower-Tool/step2/Build-the-housing/
     
   Soil Moisture Detector  (YL-69 sensor or similar) requires analog input...
     http://www.instructables.com/id/Arduino-LCD-Soil-Moisture-Sensor/step3/Connect-moisture-sensor/

   My MKR1000 Connections (All sensors must be 3.3v signalling!)
   ===========  */
int UVOUT = A0; //  Output from the MV8511 UV sensor
int lightOut = A0; //  Output from the TEMT6000 visible light sensor
int tmp36out = A0; //  Output from the TMP36 local temperature sensor
int mq9out = A0; //  Output from DFrobot MQ-9 CO/Combustable Gs sensor
/* A5  Moisture Sensor
   A6  Wind speed sensor?
   0  (HW INT)  pushbutton
   1  (HW INT)  AS  Lightning Detector
   2  (HW INT)  wind speed anemometer? (interrupt per rotation)
   3  (HW INT)  Rain Tipper…
   4 
   5   */
int sounderPin = 6; // Piezo Sounder output (shared with on-board LED)
//   (You can use a jumper to disable the sounder, but the on-board LED will blink) 
int neopixelPin = 7; //  NeoPixel output, for visualization using shift-register-based LEDs
/*11  i2c SDA
  12  i2c SCL    */
   
// Decalring variables for the Chronodot i2c RTC...
int addrRTC = (0x68);  // RTC i2c address
int seconds;   //combined BCD from the RTC (00h)
int seconds1;  //0-9
int seconds10; //0-5
int minutes;   //combined BCD from the RTC (01h)
int minutes1;  //0-9
int minutes10; //0-6
int hours;     //combined BCD from the RTC (02h)
int hours1;    //0-9 
int hours10;   //0-2
int day;       //1-7   (03h) 
int date;      //01-31 (04h)
int month;     //01-12 (05h)
int years;     //0-99; (06h)
int a1secs;    // (07h) get Alarm 1 seconds
int a1mins;    // (08h) get Alarm 1 minutes
int a1hrs;     // (09h) get Alarm 1 hours
int a1daydate; // (0Ah) get Alarm 1 day and date bits
int a2mins;    // (0Bh) get Alarm 2 minutes
int a2hrs;     // (0Ch) get Alarm 2 hours
int a2daydate; // (0Dh) get Alarm 2 day and date bits
int rtcstatus; // (0Eh) get RTC status bits
int aginginfo; // (0Fh) get aging offset info
// what is 10h?
int temprtc;   //combined BCD from the RTC  (11h)
int tempfrtc;  //combined BCD from the RTC  (12h)


/**************************************************************************/
//  Arduino setup function (automatically called at startup)
/**************************************************************************/
void setup(void) 
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("i2c Sensor Deck");

// Remember to set up your inputs and output pins!
  pinMode(UVOUT, INPUT);  // the ML8511 UV sensor
  pinMode(lightOut, INPUT);  // the TEMT6000 visible light sensor
  pinMode(tmp36out, INPUT);  // the Analog Devices TMP36 temperature sensor
  pinMode(sounderPin, OUTPUT);  // HIGH will cause the sounder to emit noise
  pinMode(neopixelPin, OUTPUT);  // Use a 220-ohm resistor in-line to the strip
  
/* Initialize the BMP085/BMP180 sensor   */
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085/180 ... check your connections */
    Serial.print("Ooops, no BMP085/180 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  /* Display some basic information on this sensor, BMP180 reports as BMP085 */
  displaySensorDetails();

/* Initialise the HTU21D sensor */
  if (!htu.begin()) {
    Serial.println("Couldn't find the HTU21-DF sensor!");
    while (1);
  }

// Initialize the Chronodot RTC clock
//(change the values, uncomment, then upload to set the time, then comment it out again)
 /* 
  seconds = 0;
  minutes = 41;
  hours = 20;
  day = 7;
  date = 3;
  month = 1;
  years = 16;
  initChrono();
*/
}

void loop(void) 
{
  beep(50, 2); // announce the start of the loop on the sounder

  // Set the neopixel to Teal...
  //int red = 0;  int green = 45;  int blue = 30;
  //strip.setPixelColor(0, (red, green, blue));
  //strip.show();

/**********************************************************************/
/*  Pull the Chronot info   i2c address 0x68 - DS1307 RTC             */
/*  Board info: http://docs.macetech.com/doku.php/chronodot           */
/*  DS3231 datasheet: http://datasheets.maxim-ic.com/en/ds/DS3231.pdf */
/**********************************************************************/
  int temprtc;
  /* Get a new timestamp */   
  Wire.beginTransmission(0x68); // 0x68 is DS3231 device address
  Wire.write((byte)0); // start at register 0
  Wire.endTransmission();
  Wire.requestFrom(0x68, 13); // request 19 bytes (is the # of bytes DEC or HEX?)
  // (seconds, minutes, hours, day, date, month, hrs, 
  //  a1secs, a1mins, a1hrs
  //  a1secs, a1mins, a1hrs
  //  Aging offset, Temp integer, temp fraction)
 
  while(Wire.available())
  { 
    seconds = Wire.read();       // (00h) get seconds
    minutes = Wire.read();       // (01h) get minutes
    hours = Wire.read();         // (02h) get hours
    day = Wire.read();           // (03h) get day of the week
    date = Wire.read();          // (04h) get date of the month
    month = Wire.read();         // (05h) get the month, and century bit
    years = Wire.read();          // (06h) get the year
    int a1secs = Wire.read();    // (07h) get Alarm 1 seconds
    int a1mins = Wire.read();    // (08h) get Alarm 1 minutes
    int a1hrs = Wire.read();     // (09h) get Alarm 1 hours
    int a1daydate = Wire.read(); // (0Ah) get Alarm 1 day and date bits
    int a2mins = Wire.read();    // (0Bh) get Alarm 2 minutes
    int a2hrs = Wire.read();     // (0Ch) get Alarm 2 hours
    int a2daydate = Wire.read(); // (0Dh) get Alarm 2 day and date bits
    int rtcstatus = Wire.read(); // (0Eh) get RTC status bits
    int aginginfo = Wire.read(); // (0Fh) get aging offset info
    temprtc = Wire.read();   // (11h) get integer portion of the temp, and sign
    tempfrtc = Wire.read();  // (12h) get the fractional portion of the temp

    //  Read our bits, and normalize the data with leading-zero padding
    // NOTE: The Chronodot doesn't know about Daylight Savings, shoulod your code?
    seconds10 = ((seconds & 0b11110000)>>4);
    seconds1 = ((seconds & 0b00001111)); 
    // convert BCD to decimal
    minutes10 = ((minutes & 0b11110000)>>4);
    minutes1 = (minutes & 0b00001111); 
    // convert BCD to decimal
    hours10 = (((hours & 0b00100000)>>5)*2 + ((hours & 0b00010000)>>4)*1);
    hours1 = (hours & 0b00001111); 
    // convert BCD to decimal (assume 24 hour mode)
    years = (years + 2000);
    temprtc = ((temprtc & 0b01111111) + (((tempfrtc & 0b11000000)>>6)*0.25));
    }  
  
  get_date();
  // This is one place you could add Dayligh Savings Time decisions to alter hours...
  Serial.print("ChronoDot - ");
  Serial.print(hours10); Serial.print(hours1); Serial.print(":"); 
  Serial.print(minutes10); Serial.print(minutes1); Serial.print(":"); 
  Serial.print(seconds10); Serial.print(seconds1); Serial.print("  20");
  Serial.print(years); Serial.print(" ");
  Serial.print(month); Serial.print(" ");
  Serial.print(date); Serial.print(" \t");
  Serial.print(temprtc); Serial.println(" C");
  delay(100);  // so this will finish printing, in case the next sensor is stalled

  
/**********************************************************************/
/*  Get BMP180 data    i2c address 0x77 - BMP180 Baro Pres and Temp   */
/*  data: http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf  */
/**********************************************************************/
  sensors_event_t event;
  bmp.getEvent(&event);
 
    /* First we get the current temperature from the BMP085/BMP180       */
    float BMPtemperature;
    bmp.getTemperature(&BMPtemperature); 
    float BMPtempF = (BMPtemperature * 1.8 + 32);
    Serial.print("Temp: ");
    Serial.print(BMPtemperature);
    Serial.print(" C (");
    Serial.print(BMPtempF);
    Serial.print(" F) \t");
    
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
    /* Display atmospheric pressue in hPa */
    Serial.print("BMP180 - Pres: ");
    Serial.print(event.pressure);
    Serial.print(" hPa\t");
    
    /* Calculating altitude with reasonable accuracy requires pressure    *
     * sea level pressure for your position at the moment the data is     *
     * converted, as well as the ambient temperature in degress           *
     * celcius.  If you don't have these values, a 'generic' value of     *
     * 1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA   *
     * in sensors.h), but this isn't ideal and will give variable         *
     * results from one day to the next.                                  *
     *                                                                    *
     * You can usually find the current SLP value by looking at weather   *
     * websites or from environmental information centers near any major  *
     * airport.                                                           *
     *                                                                    *
     * convert inches-mercury http://www.csgnetwork.com/pressinmbcvt.html *
     *
     * For example, for Paris, France you can check the current mean      *
     * pressure and sea level at: http://bit.ly/16Au8ol                   */
     
    /* Then convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    Serial.print("Alt: "); 
    Serial.print(bmp.pressureToAltitude(seaLevelPressure,
                                        event.pressure)); 
    Serial.println(" m");
    delay(100);  // so this will finish printing, in case the next sensor is stalled
  }
  else
  {
    Serial.println("Sensor error");
  }
  

/**********************************************************************
/* Get HTU21-DF data    i2c address 0x40 -  Humidity and Temp Sensor  *
/*   Then convert the atmospheric pressure, and SLP to altitude       *
/*   Update this next line with the current SLP for better results    *
/*   https://learn.adafruit.com/adafruit-htu21d-f-temperature-humidity-sensor/overview
/**********************************************************************/
    float HTUtemperature = htu.readTemperature(); 
    float HTUtempF = (HTUtemperature * 1.8 + 32);
  Serial.print("HTU21-DF - Temp: "); Serial.print(HTUtemperature);
  Serial.print(" C ("); Serial.print(HTUtempF);
  Serial.print(" F)\tHum: "); Serial.print(htu.readHumidity());
  Serial.println("%");
  delay(100);  // so this will finish printing, in case the next sensor is stalled


/**********************************************************************
/* Analog Devices venerable TMP36 precision temperature sensor
/*  this requires a bit of math after reading the output...
/*  https://learn.adafruit.com/tmp36-temperature-sensor/using-a-temp-sensor
/**********************************************************************/
 //getting the voltage reading from the temperature sensor
 int reading = averageAnalogRead(tmp36out);  
 
 // 0.0032258064516129 are the DAC unit for 3.3v 
 float tmp36voltage = 0.0032258064516129 * reading;
 
 // print out the voltage
 Serial.print("TMP36 - temp: ");
 float tmp36temperatureC = (tmp36voltage - 0.5) * 100 ;
 //converting from 10 mv per degree with 500 mV offset to degrees ((voltage - 500mV) times 100)
 Serial.print(tmp36temperatureC); Serial.print(" C \t");
 
 // now convert to Fahrenheit
 float tmp36temperatureF = (tmp36temperatureC * 9.0 / 5.0) + 32.0;
 Serial.print(tmp36temperatureF); Serial.print(" F, out: ");

 Serial.print(tmp36voltage); Serial.println("v");
  delay(100);  // so this will finish printing, in case the next sensor is stalled


/**********************************************************************
 * Vishay TEMT6000 Visible Light sensor - analog reading
 * https://www.sparkfun.com/products/8688
/**********************************************************************/  
  int vLevel = averageAnalogRead(lightOut);
  // 0.0032258064516129 is (3.3v (the DAC ref voltage) \ 1023 * uvLevel)
  float newVOutVolts = 0.0032258064516129 * vLevel;

  Serial.print("TEMT6000 out: ");
  Serial.println(vLevel);
  delay(100);  // so this will finish printing, in case the next sensor is stalled
  
  
/**********************************************************************
 * ML8511 UV Sensor  - analog reading
 * https://learn.sparkfun.com/tutorials/ml8511-uv-sensor-hookup-guide *
/**********************************************************************/  
  int uvLevel = averageAnalogRead(UVOUT);
  // 0.0032258064516129 is (3.3v (the DAC ref voltage) \ 1023 * uvLevel)
  float newOutVolts = 0.0032258064516129 * uvLevel;  
  
  //Convert the voltage to a UV intensity level
  float uvIntensity = mapfloat(newOutVolts, 0.99, 2.8, 0.0, 15.0); 

  Serial.print("ML8511 UV out: ");
  Serial.print(uvLevel);

  Serial.print(" / UV Intensity (mW/cm^2): ");
  Serial.println(uvIntensity);
  delay(100);  // so this will finish printing, in case the next sensor is stalled


/**********************************************************************
 * DFrobot MQ-9 CO/Combustable Gas sensor - analog reading
 * http://www.dfrobot.com/wiki/index.php/Analog_Gas_Sensor(MQ9)_(SKU:SEN0134)
 * https://www.pololu.com/category/83/gas-sensors  There are many available
 * But, deciphering what the output levels mean is an exercise for the buyer. :-(
/**********************************************************************/
  int MQ9volts = analogRead(mq9out);  // Read Gas value from the MQ-9 sensor
  Serial.print("MQ-9 Gas: "); Serial.println(MQ9volts,DEC);
  delay(100);  // so this will finish printing, in case the next sensor is stalled



  Serial.println("");
  delay(3500); // looking to time the loop at about 5 seconds...
  // End of the main loop...
}

/**************************************************************************/
/*  The code below are supporting subroutines                             *
/**************************************************************************/
/* Chronodot-related subroutines                                          *
 *   initChrono, set_date, get_date, set_time, get_time, get_temp,        *
 *   setHour, SetMinutes, decToBcd, bcdToDec                              */
/**************************************************************************/
void initChrono()
{
  set_time();
  set_date();
}

void set_date()
{
  Wire.beginTransmission(104);
  Wire.write(4);
  Wire.write(decToBcd(day));
  Wire.write(decToBcd(date));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(years));
  Wire.endTransmission();
}

void get_date()
{
  Wire.beginTransmission(104);
  Wire.write(3);//set register to 3 (day)
  Wire.endTransmission();
  Wire.requestFrom(104, 4); //get 4 bytes(day,date,month,year);
  day   = bcdToDec(Wire.read());
  date  = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  years  = bcdToDec(Wire.read());
}

void set_time()
{
  Wire.beginTransmission(104);
  Wire.write((byte)0);
  Wire.write(decToBcd(seconds));
  Wire.write(decToBcd(minutes));
  Wire.write(decToBcd(hours));
  Wire.endTransmission();
}

void get_time()
{
  Wire.beginTransmission(104);
  Wire.write((byte)0);//set register to 0
  Wire.endTransmission();
  Wire.requestFrom(104, 3);//get 3 bytes (seconds,minutes,hours);
  seconds = bcdToDec(Wire.read() & 0x7f);
  minutes = bcdToDec(Wire.read());
  hours = bcdToDec(Wire.read() & 0x3f);
}

void get_temp()
{
  Wire.beginTransmission(104);
  Wire.write((byte)0);  //set register to 0
  Wire.endTransmission();
  Wire.requestFrom(104, 3);//get 3 bytes (seconds,minutes,hours);
  seconds = bcdToDec(Wire.read() & 0x7f);
  minutes = bcdToDec(Wire.read());
  hours = bcdToDec(Wire.read() & 0x3f);
}

void setHour()
{
  hours++;
  if (hours > 23)
  {
    hours = 0;
    seconds = 0;
    minutes = 0;
  }
  set_time();

}
void setMinutes()
{
  minutes++;
  if (minutes > 59)
  {
    minutes = 0;

  }
  seconds = 0;

  set_time();
}

byte decToBcd(byte val)
{
  return ( (val / 10 * 16) + (val % 10) );
}

byte bcdToDec(byte val)
{
  return ( (val / 16 * 10) + (val % 16) );
}

/**************************************************************************/
/*   Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)   */
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
//  bmp.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/* Takes an average of readings on a given pin, Returns the average       *
/*   used for the TMP36 and ML8511 UV Sensor readings.
/**************************************************************************/

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
    runningValue /= numberOfReadings;
    return(runningValue);  
}

//The Arduino Map function but for floats
//From: http://forum.arduino.cc/index.php?topic=3922.0
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void beep(int duration, int count)
{
  for(int loop = 0 ; loop < count ; loop++) {
    digitalWrite(sounderPin, HIGH);
    delay(duration);
    digitalWrite(sounderPin, LOW);
    delay(duration);
  }
}
