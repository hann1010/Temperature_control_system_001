
/* Temperature_control_system_001 Hanna P 2020
For Arduino Mega 2560 board */
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


#include <SoftwareSerial.h> // Comes with Arduino IDE


/*-----( Declare Constants )-----*/
const int powerOn = 2;   // Pin for set Sim900 power on
const int testButton = 3;   // Pin for send test botton


/*-----( Declare objects )-----*/
// set the LCD address to 0x20 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
// Set the LCD I2C address 0x20, 0x3f or something else,
// depending on the I2C circuit
LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 temperature_sensor_heating_burner = Adafruit_MAX31865(10, 11, 12, 13);
Adafruit_MAX31865 temperature_sensor_heating_tanktop = Adafruit_MAX31865(9, 11, 12, 13);
Adafruit_MAX31865 temperature_sensor_heating_inline = Adafruit_MAX31865(8, 11, 12, 13);
Adafruit_MAX31865 temperature_sensor_heating_hotwater = Adafruit_MAX31865(7, 11, 12, 13);
Adafruit_MAX31865 temperature_sensor_outdoor = Adafruit_MAX31865(6, 11, 12, 13);
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


/*-----------------------------------------------------------------
Note for SoftwareSerial:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
-------------------------------------------------------------------*/
SoftwareSerial mySerial(51, 50); // 3=TX 2=RX in GSM shield side Arduino uno only


/*-----( Declare Variables )-----*/


String phoneNro = "";
String sendTitle = "";

int numOfMsgSend = 0;

float heating_burner;
float heating_tanktop;
float heating_inline;
float heating_hotwater;
float outdoor;

boolean sendRequest = false;
boolean sendDone = true;

void setup()  /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);  // initialize the hardware UART for speed 9600
  
  digitalWrite(powerOn, LOW); // Set power sw pin to Low
  pinMode(testButton, INPUT_PULLUP); // initialize the digital pins to input and pullup modes
  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines
  
  temperature_sensor_heating_burner.begin(MAX31865_2WIRE);  // Set to 2WIRE or 4WIRE as necessary
  temperature_sensor_heating_tanktop.begin(MAX31865_2WIRE); 
  temperature_sensor_heating_inline.begin(MAX31865_2WIRE);
  temperature_sensor_heating_hotwater.begin(MAX31865_2WIRE);
  temperature_sensor_outdoor.begin(MAX31865_2WIRE);

  lcd.backlight(); // set backlight on 
 
//-------- Write characters on the display ----------------
// NOTE: Cursor Position: CHAR, LINE) start at 0 
  lcd.setCursor(0,1); //Start at character 0 on line 2
  lcd.print("Booting up system...");
  delay(1000);
  setupReceiveSMS(); // Setting up Sim900 for Arduino mega 2560 board
  lcd.clear();
  lcd.setCursor(0,1); //Start at character 0 on line 2
  lcd.print("Temperature control system version.001");
  delay(5000);
  lcd.clear();
} /*--(end setup )---*/

void loop() /*----( LOOP: RUNS CONSTANTLY )----*/
{
  /*----------Print some values to terminal-----------*/
  //Serial.print("outdoor temperature = "); Serial.println(Temperature_sensor_outdoor.temperature(RNOMINAL, RREF_outdoor));
  

  /*----------Print to LCD-----------*/
  for(int i = 0; i< 10; i++)
  {
    
    sendTestSMS(); // Reading test button and if pressed send test SMS
    temperatureRead(); // Reading temperature from sensor)
    sendAlarm() // Reading temperature values and if necessary send SMS
{
    if (i < 5) 
      {
        //lcd.clear();
        if ( i == 0)
            {
              lcd.clear();
            } 
        lcd.setCursor(0,0);
        lcd.print("Burner = "); lcd.print(heating_burner);
        lcd.setCursor(0, 1);
        lcd.print("Inline = "); lcd.print(heating_inline);
        //lcd.setCursor(10, 1);
        //lcd.print("");
        //lcd.setCursor(12, 2);
        lcd.setCursor(0, 2);
        lcd.print("Hot water = "); lcd.print(heating_hotwater);
        lcd.setCursor(0, 3);
        lcd.print("Out temp = "); lcd.print(outdoor);
        //lcd.print(" = "); lcd.print(max2.temperature(RNOMINAL2, RREF2));   
        delay(1000);                       
      }
    else
      {
        if ( i == 5)
            {
              lcd.clear();
            }      
        //lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Burner = "); lcd.print(heating_burner);
        lcd.setCursor(0,1);
        lcd.print("Tank top = "); lcd.print(heating_tanktop);
        lcd.setCursor(0,2);
        lcd.print("Phone NO = "); lcd.print(phoneNro);
        lcd.setCursor(0,3); 
        lcd.print("Num of Msg send = "); lcd.print(numOfMsgSend);
        //lcd.clear(); 
//        lcd.print("str "); lcd.print(inputString.substring(90,130));
        delay(1000);
      }
  }

} /* --(end main loop )-- */


void setupReceiveSMS()
{
  /* Setting up Sim900 for Arduino mega 2560 board
  ------------------------------------------------*/
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  //Serial1.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino uno only)
  delay(5000);          // Waiting for Sim900 booting up
  mySerial.println("AT+CPIN=\"9009\"\r"); // Set Pin code
  Serial.println("Recieve Setup... Done "); //(Arduino uno only)

  mySerial.println("AT+CMGF=1\r"); // turn to text mode
  mySerial.println("AT+CMGR=ALL\r"); // read all SMS from the inbox
  mySerial.println("AT+CMGD=1,1"); // Delete all read SMS from Sim card
  mySerial.println("AT+CMGL=ALL\r"); // AT Command to receive a live SMS
  Serial.println("SMS Setup... Done "); //(Arduino uno only)
  
}



void temperatureRead()
{
  /* Reading temperature from sensors
  -------------------------------------------*/
  heating_burner = temperature_sensor_heating_burner.temperature(RNOMINAL, RREF_burner);
  heating_tanktop = temperature_sensor_heating_tanktop.temperature(RNOMINAL, RREF_tanktop);
  heating_inline = temperature_sensor_heating_inline.temperature(RNOMINAL, RREF_inline);
  heating_hotwater = temperature_sensor_heating_hotwater.temperature(RNOMINAL, RREF_hotwater);
  outdoor = temperature_sensor_outdoor.temperature(RNOMINAL, RREF_outdoor);
  //Debug
  /*
  Serial.print("Burner = ");
  Serial.println(heating_burner);
  Serial.print("Tank top = ");
  Serial.println(heating_tanktop);
  Serial.print("Inline = ");
  Serial.println(heating_inline);
  Serial.print("Hot water = ");
  Serial.println(heating_hotwater);
  Serial.print("Out temp = ");
  Serial.println(outdoor);
  
  Serial.print("numOfMsgRecieve = ");
  Serial.println(numOfMsgRecieve);
  */
}


void sendTestSMS()
{
    /* Sending SMS test message.
  -------------------------------------------*/
  int buttonState = digitalRead(testButton);
  if (buttonState == LOW)
  {
    Serial.println("Sending test... ");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Sending test...");
    phoneNro = "+12345";
    sendTitle = "test";
    sendSMS();
  }

}

void sendAlarm()
{
    /* Reading temperature values and if necessary send alarm SMS message.
  -----------------------------------------------------------------------*/

}

void sendSMS()
{
  // Debug
  Serial.println("Send test... ");
  Serial.print("PhoneNro = ");
  Serial.println(phoneNro);
  Serial.print("SendTitle = ");
  Serial.println(sendTitle);
  Serial.print("Burner = ");
  Serial.println(heating_burner);
  Serial.print("Tank top = ");
  Serial.println(heating_tanktop);
  Serial.print("Inline = ");
  Serial.println(heating_inline);
  Serial.print("Hot water = ");
  Serial.println(heating_hotwater);
  Serial.print("Out temp = ");
  Serial.println(outdoor);
  /* Send SMS messages
  ------------------------------------------- */
  
  //Serial.print("Send message... "); //(Arduino uno only)
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
 // mySerial.println("AT+CMGS=\"x\"\r"); // Replace x with mobile number
  mySerial.print("AT+CMGS="); // Set phone nro 
  mySerial.print(char(34));  //***  ASCII code of " (char(34)
  mySerial.print(phoneNro);  //***
  mySerial.println(char(34)); // End of setting phone nro with " (char(34)
  delay(1000);
  mySerial.print("SendTitle = ");// The SMS text you want to send
  mySerial.println(sendTitle);
  mySerial.print("Burner = ");
  mySerial.println(heating_burner);
  mySerial.print("Tank top = ");
  mySerial.println(heating_tanktop);
  mySerial.print("Inline = ");
  mySerial.println(heating_inline);
  mySerial.print("Hot water = ");
  mySerial.println(heating_hotwater);
  mySerial.print("Out temp = ");
  mySerial.println(outdoor);
  delay(1000);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println("The message has been sent");
//  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print("The message has been sent");
  delay(2000);
 
}
