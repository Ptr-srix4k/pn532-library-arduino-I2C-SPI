#include <Wire.h>
#include <SPI.h>
#include "PN532_srix4k.h"

#define PN532_SS   10
#define USE_SPI   0

void setup() {
  
  if (USE_SPI)
  {
    SPI.begin();
    PN532_use_spi(PN532_SS);
  }
  else
  {
    Wire.begin(); 
    Wire.setClock(100000);
  }
  Serial.begin(9600);
}

void loop() {
  
  uint8_t chip_id;
  uint8_t block[] = {0x00,0x00,0x00,0x00};
  uint8_t UID[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

  int i;

  //========================================================
  // PN532 CONFIGURATION
  //========================================================
  
  if (!PN532_SAMConfiguration())
  {
    Serial.println("Error: PN532_SAMConfiguration command fail");
    while(1); 
  }
  if (!PN532_GetFirmwareVersion())
  {
    Serial.println("Error: PN532_GetFirmwareVersion command fail");
    while(1); 
  }
  if (!PN532_RFconfiguration())
  {
    Serial.println("Error: PN532_RFconfiguration command fail");
    while(1); 
  }
  if (!PN532_InlistPassiveTarget())
  {
    Serial.println("Error: PN532_InlistPassiveTarget command fail");
    while(1); 
  }

  //========================================================
  // SRIX4K COMMANDS
  //========================================================
  
  Serial.println("Initiate command");
  if (!PN532_Initiate(&chip_id))
  {
    Serial.println("Error: PN532_Initiate command fail");
    while(1); 
  }

  Serial.println("Select chip ID");
  if (!PN532_Select(chip_id))
  {
    Serial.println("Error: PN532_Select command fail");
    while(1); 
  }
  
  Serial.println("Read Blocks");
  for (i=0;i<128;i++)
  {
    Serial.print(i,HEX);
    Serial.print(" : ");
    if (!PN532_ReadBlock(i,block))
    {
        Serial.println("Error: PN532_ReadBlock command fail");
        break;
    }
    Serial.print(block[0], HEX); Serial.print(" ");
    Serial.print(block[1], HEX); Serial.print(" ");
    Serial.print(block[2], HEX); Serial.print(" ");
    Serial.print(block[3], HEX); Serial.println(" "); 
  }

  /*
  Serial.println("Write Block");
  block[0] = 0x11;
  block[1] = 0x22;
  block[2] = 0x33;
  block[3] = 0x44; 
  if (!PN532_WriteBlock(0x58,block))
  {
    Serial.println("Error: PN532_ReadBlock command fail");
    break;
  }
  */

  if (!PN532_GetUID(UID))
  {
    Serial.println("Error: PN532_GetUID command fail");
    while(1);
  }
  Serial.print("UID : ");
  Serial.print(UID[0], HEX); Serial.print(" ");
  Serial.print(UID[1], HEX); Serial.print(" ");
  Serial.print(UID[2], HEX); Serial.print(" ");
  Serial.print(UID[3], HEX); Serial.print(" "); 
  Serial.print(UID[4], HEX); Serial.print(" "); 
  Serial.print(UID[5], HEX); Serial.print(" "); 
  Serial.print(UID[6], HEX); Serial.print(" "); 
  Serial.print(UID[7], HEX); Serial.print(" "); 
  Serial.println(""); 

  while(1);
}

void wait_serial_keypress()
{
  while(Serial.available() == 0) {}
    int mydata = Serial.read();
}
