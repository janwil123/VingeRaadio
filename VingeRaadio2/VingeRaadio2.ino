#include <Si4703_Breakout.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

// Pins and inital values for the radio

int resetPin = 2;
int SDIO = A4;
int SCLK = A5;

Si4703_Breakout radio(resetPin, SDIO, SCLK);
int channel=994;
int volume=3;
char rdsBuffer[10];

// Pins for the LCD

//const int rs = 12, en = 11, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pins for the bush buttons

const int button1 = 4, button2 = 5, button4 = 6, button8 = 7;

void setup()
{
  Serial.begin(9600);
  Serial.println("\n\nSi4703_Breakout Test Sketch");
  Serial.println("===========================");  
  Serial.println("a b     Favourite stations");
  Serial.println("+ -     Volume (max 15)");
  Serial.println("u d     Seek up / down");
  Serial.println("r       Listen for RDS Data (15 sec timeout)");
  Serial.println("Send me a command letter.");
  

  radio.powerOn();
  Serial.println("Chip booted.");
  radio.setChannel(channel);
  radio.setVolume(volume);
  displayInfo();

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Hello, world!");

  pinMode(button1, INPUT);
  digitalWrite(button1, HIGH);
  pinMode(button2, INPUT);
  digitalWrite(button2, HIGH);
  pinMode(button4, INPUT);
  digitalWrite(button4, HIGH);
  pinMode(button8, INPUT);
  digitalWrite(button8, HIGH);
}

void loop()
{ 
  

  
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == 'u') 
    {
      channel = radio.seekUp();
      displayInfo();
    } 
    else if (ch == 'd') 
    {
      channel = radio.seekDown();
      displayInfo();
    } 
    else if (ch == '+') 
    {
      volume ++;
      if (volume == 16) volume = 15;
      radio.setVolume(volume);
      displayInfo();
    } 
    else if (ch == '-') 
    {
      volume --;
      if (volume < 0) volume = 0;
      radio.setVolume(volume);
      displayInfo();
    } 
    else if (ch == 'a')
    {
      channel = 986; // Retro FM
      radio.setChannel(channel);
      displayInfo();
    }
    else if (ch == 'b')
    {
      channel = 1036; // Raadio 2
      radio.setChannel(channel);
      displayInfo();
    }
    else if (ch == 'c')
    {
      channel = 1057; // Klassikaraadio
      radio.setChannel(channel);
      displayInfo();
    }
    else if (ch == 'r')
    {
      Serial.println("RDS listening");
      radio.readRDS(rdsBuffer, 30000);
      Serial.print("RDS heard:");
      Serial.println(rdsBuffer);      
    }
  }
}

void displayInfo()
{
   Serial.print("Channel:"); Serial.print(channel); 
   Serial.print(" Volume:"); Serial.println(volume); 
}
