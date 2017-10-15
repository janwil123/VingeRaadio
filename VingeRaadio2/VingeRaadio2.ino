#include <Si4703_Breakout.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

// Pins and inital values for the radio

int resetPin = 2;
int SDIO = A4;
int SCLK = A5;

Si4703_Breakout radio(resetPin, SDIO, SCLK);
char rdsBuffer[10];

// Pins for the bush buttons

const int button1Pin = 4;
const int button2Pin = 5;
const int button4Pin = 6;
const int button8Pin = 7;// the number of the pushbutton pin

int button1State = 0;
int button2State = 0;
int button4State = 0;
int button8State = 0;

int pinsState[4] = {0,0,0,0};

bool pinsPressed = false;

const int jaamasid=15;

const int sagedused[jaamasid] = { 
  885,
  890,
  897,
  903,
  944,
  952,
  986,
  994,
  1002,
  1030,
  1036,
  1047,
  1057,
  1061,
  1067};
                     
const char* jaamanimed[jaamasid]={
  "Raadio Elmar    ",
  "Tartu Pereraadio",
  "PWR Hit Radio   ",
  "Klassikaraadio  ",
  "Raadio 4        ",
  "Sky Plus        ",
  "Retro FM        ",
  "Star FM         ",
  "Raadio Kuku     ",
  "Hit FM          ",
  "Raadio 2        ",
  "Ring FM         ",
  "Klassikaraadio  ",
  "Vikerraadio     ",
  "Vikerraadio     "};

int jaam = 0;

void setup()
{
  Serial.begin(9600);

  radio.powerOn();
  Serial.println("Chip booted.");
  radio.setVolume(15);
  lcd.begin(16, 2);
  vahetaJaama(0);

  pinMode(button1Pin, INPUT);
  digitalWrite(button1Pin, HIGH);
  pinMode(button2Pin, INPUT);
  digitalWrite(button2Pin, HIGH);
  pinMode(button4Pin, INPUT);
  digitalWrite(button4Pin, HIGH);
  pinMode(button8Pin, INPUT);
  digitalWrite(button8Pin, HIGH);
}

void loop()
{ 
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  button4State = digitalRead(button4Pin);
  button8State = digitalRead(button8Pin);

  //Serial.println(button1State);
  
  if ((button1State==LOW) || (button2State==LOW) || (button4State==LOW) || (button8State==LOW)){
    if (button1State==LOW) {
      Serial.println("Vajutati nupp 1");
    }
    if (button2State==LOW) {
      Serial.println("Vajutati nupp 2");
    }
    if (button4State==LOW) {
      Serial.println("Vajutati nupp 4");
    }
    if (button8State==LOW) {
      Serial.println("Vajutati nupp 8");
    }
    
    pinsPressed = true;
    pinsState[0] = (pinsState[0] || (button1State==LOW));
    pinsState[1] = (pinsState[1] || (button2State==LOW));
    pinsState[2] = (pinsState[2] || (button4State==LOW));
    pinsState[3] = (pinsState[3] || (button8State==LOW));
  }
  else {
    if (pinsPressed){
      jaam = pinsState[0] + 2*pinsState[1] + 4*pinsState[2] + 8*pinsState[3]-1;
      Serial.println(jaam);
      //Change the station
      vahetaJaama(jaam);
      //Reset pins
      pinsState[0] = 0;
      pinsState[1] = 0;
      pinsState[2] = 0;
      pinsState[3] = 0;
      pinsPressed = false;
    }
  }
}

void vahetaJaama(int jaam){
      radio.setChannel(sagedused[jaam]);
      //Display station number and name
      lcd.setCursor(0,0);
      lcd.print(String(jaam+1)+". "+jaamanimed[jaam]);
      // 8th character - 2nd line 
      lcd.setCursor(8,1);
      lcd.print(String(sagedused[jaam]/10.0)+" ");
}

