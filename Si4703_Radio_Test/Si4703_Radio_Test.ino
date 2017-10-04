#include <Si4703_Breakout.h>
#include <Wire.h>

int resetPin = 2;
int SDIO = A4;
int SCLK = A5;

Si4703_Breakout radio(resetPin, SDIO, SCLK);
int channel=994;
int volume=3;
char rdsBuffer[10];

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
    else if (ch == 'd')
    {
      channel = 994; // Star FM
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
