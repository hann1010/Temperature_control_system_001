
/* Temperature_control_system_001 Hanna P 2020 */
/********************************************/

/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here:
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.

#include <LiquidCrystal_I2C.h>

/*************************************************** 
  This is a library for the Adafruit PT100/P1000 RTD Sensor w/MAX31865
  Designed specifically to work with the Adafruit RTD Sensor
  ----> https://www.adafruit.com/products/3328
  This sensor uses SPI to communicate, 4 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_MAX31865.h>

/*-----( Declare Constants )-----*/

/*-----( Declare objects )-----*/
// set the LCD address to 0x20 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
// Set the LCD I2C address 0x20, 0x3f or something else,
// depending on the I2C circuit
LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 Temperature_sensor_heating_burner = Adafruit_MAX31865(10, 11, 12, 13);
Adafruit_MAX31865 Temperature_sensor_heating_tanktop = Adafruit_MAX31865(9, 11, 12, 13);
Adafruit_MAX31865 Temperature_sensor_heating_inline = Adafruit_MAX31865(8, 11, 12, 13);
Adafruit_MAX31865 Temperature_sensor_heating_hotwater = Adafruit_MAX31865(7, 11, 12, 13);
Adafruit_MAX31865 Temperature_sensor_outdoor = Adafruit_MAX31865(6, 11, 12, 13);
// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31865 max = Adafruit_MAX31865(10);

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF_burner      427.0
#define RREF_tanktop      427.0
#define RREF_inline      427.0
#define RREF_hotwater      427.0
#define RREF_outdoor      427.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0



/*-----( Declare Variables )-----*/
//none

void setup()  /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);  //initialize the hardware UART for speed 9600

  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines
  
  Temperature_sensor_heating_burner.begin(MAX31865_2WIRE);  // set to 2WIRE or 4WIRE as necessary
  Temperature_sensor_heating_tanktop.begin(MAX31865_2WIRE); 
  Temperature_sensor_heating_inline.begin(MAX31865_2WIRE);
  Temperature_sensor_heating_hotwater.begin(MAX31865_2WIRE);
  Temperature_sensor_outdoor.begin(MAX31865_2WIRE);

  lcd.backlight(); // set backlight on 
 
//-------- Write characters on the display ----------------
// NOTE: Cursor Position: CHAR, LINE) start at 0 
  lcd.setCursor(0,1); //Start at character 0 on line 2
  lcd.print("Temperature control system 001");
  delay(5000);
  lcd.clear();

} /*--(end setup )---*/

void loop() /*----( LOOP: RUNS CONSTANTLY )----*/
{
  /*----------Print some values to terminal-----------*/
  Serial.print("outdoor temperature = "); Serial.println(Temperature_sensor_outdoor.temperature(RNOMINAL, RREF_outdoor));
  
  /*----------Print to LCD-----------*/
  for(int i = 0; i< 10; i++)
  {
    if (i < 5) 
      {
        //lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Burner = "); lcd.print(Temperature_sensor_heating_burner.temperature(RNOMINAL, RREF_burner));
        lcd.setCursor(0, 1);
        lcd.print("Inline = "); lcd.print(Temperature_sensor_heating_inline.temperature(RNOMINAL, RREF_inline));
        //lcd.setCursor(10, 1);
        //lcd.print("");
        //lcd.setCursor(12, 2);
        lcd.setCursor(0, 2);
        lcd.print("Hot water = "); lcd.print(Temperature_sensor_heating_hotwater.temperature(RNOMINAL, RREF_hotwater));
        lcd.setCursor(0, 3);
        lcd.print("Out temp = "); lcd.print(Temperature_sensor_outdoor.temperature(RNOMINAL, RREF_outdoor));
        //lcd.print(" = "); lcd.print(max2.temperature(RNOMINAL2, RREF2));   
        delay(1000);                       
      }
    else
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Burner = "); lcd.print(Temperature_sensor_heating_burner.temperature(RNOMINAL, RREF_burner));
        lcd.setCursor(0,1);
        lcd.print("Tank top = "); lcd.print(Temperature_sensor_heating_tanktop.temperature(RNOMINAL, RREF_tanktop));
        delay(1000);
      }
  }
  
  /*lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Burner = "); lcd.print(Temperature_sensor_heating_burner.temperature(RNOMINAL, RREF_burner));
  lcd.setCursor(0, 1);
  lcd.print("Inline = "); lcd.print(Temperature_sensor_heating_inline.temperature(RNOMINAL, RREF_inline));
  //lcd.setCursor(10, 1);
  //lcd.print("");
  //lcd.setCursor(12, 2);
  lcd.setCursor(0, 2);
  lcd.print("Hot water = "); lcd.print(Temperature_sensor_heating_hotwater.temperature(RNOMINAL, RREF_hotwater));
  lcd.setCursor(0, 3);
  lcd.print("Out temp = "); lcd.print(Temperature_sensor_outdoor.temperature(RNOMINAL, RREF_outdoor));
  //lcd.print(" = "); lcd.print(max2.temperature(RNOMINAL2, RREF2));
  delay(1000);*/
  
  

} /* --(end main loop )-- */
