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

int sagedused[9] = { 1067,
                     1036, 
                     1057, 
                     1047, 
                     994, 
                     885, 
                     1002, 
                     952, 
                     986};
                     
char* jaamanimed[9]={"Vikerraadio     ", 
                     "Raadio 2        ", 
                     "Klassikaraadio  ", 
                     "Ring FM         ", 
                     "Star FM         ",
                     "Raadio Elmar    ", 
                     "Raadio Kuku     ",
                     "Sky Plus        ",
                     "Retro FM        "};

int jaam = 0;

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

  Serial.println(button1State);
  
  if ((button1State==LOW) || (button2State==LOW) || (button4State==LOW) || (button8State==LOW)){
    Serial.println("Midagi vajutati");
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
      pinsState[0] = 0;
      pinsState[1] = 0;
      pinsState[2] = 0;
      pinsState[3] = 0;
      pinsPressed = false;
    }
  }
  
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
