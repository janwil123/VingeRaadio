///
/// \file  TestTEA5767.ino
/// \brief An Arduino sketch to operate a TEA5767 chip based radio using the Radio library.
///
/// \author Matthias Hertel, http://www.mathertel.de
/// \copyright Copyright (c) 2014 by Matthias Hertel.\n
/// This work is licensed under a BSD style license.\n
/// See http://www.mathertel.de/License.aspx
///
/// \details
/// This sketch implements a "as simple as possible" radio without any possibility to modify the settings after initializing the chip.\n
/// The radio chip is initialized and setup to a fixed band and frequency. These settings can be changed by modifying the 
/// FIX_BAND and FIX_STATION definitions. 
///
/// Open the Serial console with 57600 baud to see the current radio information.
///
/// Wiring
/// ------ 
/// Arduino port | Si4703 signal
/// ------------ | ---------------
///     3.3V | VCC
///      GND | GND
///       A5 | SCLK
///       A4 | SDIO
///       D2 | RST
///
/// More documentation and source code is available at http://www.mathertel.de/Arduino
///
/// History:
/// --------
/// * 15.09.2014 created.

#include <Arduino.h>
#include <Wire.h>
#include <radio.h>
#include <TEA5767.h>
#include <LiquidCrystal_I2C.h> // Using version 1.2.1

/// The band that will be tuned by this sketch is FM.
#define FIX_BAND RADIO_BAND_FM

#define JAAMADE_ARV 9

TEA5767 radio;    // Create an instance of Class for Si4703 Chip

uint8_t test1;
byte test2;

const int buttonPin = 2;     // the number of the pushbutton pin

int buttonState = HIGH;         // variable for reading the pushbutton status

int lastButtonState = HIGH;

int sagedused[JAAMADE_ARV] = {10670, 10360, 10570, 10470, 9940, 8850, 10020, 9520, 9860};
char* jaamanimed[JAAMADE_ARV]={"Vikerraadio     ", 
                     "Raadio 2        ", 
                     "Klassikaraadio  ", 
                     "Ring FM         ", 
                     "Star FM         ",
                     "Raadio Elmar    ", 
                     "Raadio Kuku     ",
                     "Sky Plus        ",
                     "Retro FM        "};

int jaam = 0;

LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

/// Setup a FM only radio configuration
/// with some debugging on the Serial port
void setup() {
  // open the Serial port
  Serial.begin(57600);
  Serial.println("Radio...");
  delay(200);

  // Initialize the Radio 
  radio.init();

  // Enable information to the Serial port
  radio.debugEnable();

  // HERE: adjust the frequency to a local sender
  radio.setBandFrequency(FIX_BAND, sagedused[0]); // Esimene jaam
  radio.setVolume(2);
  radio.setMono(false);

    // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  lcd.begin(16,2); // sixteen characters across - 2 lines
  lcd.backlight();
} // setup


/// show the current chip data every 3 seconds.
void loop() {
  char s[12];
  radio.formatFrequency(s, sizeof(s));
  Serial.print("Station:"); 
  Serial.println(s);
  
  Serial.print("Radio:"); 
  radio.debugRadioInfo();
  
  Serial.print("Audio:"); 
  radio.debugAudioInfo();

  buttonState = digitalRead(buttonPin);

  if (lastButtonState == LOW) {
    if (buttonState == HIGH) {
      // button pressed
      lastButtonState=HIGH;
      Serial.print("Button pressed");
      jaam = (jaam + 1) % JAAMADE_ARV; 
      radio.setFrequency(sagedused[jaam]);
      lcd.setCursor(0,0);
      lcd.print(jaamanimed[jaam]);
      // 8th character - 2nd line 
      lcd.setCursor(8,1);
      lcd.print(String(sagedused[jaam]/100.0)+" ");
    } 
  } else { //lastButtonState == HIGH
    if (buttonState == LOW) {
      lastButtonState = LOW;
    }
  }
  //delay(500);
} // loop

// End.

