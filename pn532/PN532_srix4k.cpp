#include "PN532_srix4k.h"
#include <Wire.h>
#include <SPI.h>

char readdata_err[] = "readdata not one";
char ack_err[] = "ACK not correct";
char data_err[] = "DATA not correct";
char use_spi = 0;
char _ss = 0;

void PN532_use_spi(uint8_t ss)
{
    use_spi = 1;
    _ss = ss;
    pinMode(_ss, OUTPUT);
}

uint8_t PN532_SAMConfiguration()
{
    uint8_t WriteData[4];
    uint8_t ReadData[9];

    WriteData[0] = 0x14;
    WriteData[1] = 0x01; ///Normal Mode
    WriteData[2] = 0x00; ///No Timeout
    WriteData[3] = 0x00; ///IRQ at high level

    //Send Command
    writecommand(WriteData, 4, 1); //Wake=1 for SPI
    //Check Ack
    if(checkAck())
    { 
      //Read Data
      if(!readdata(ReadData,2+7,TIMEOUT)) //Read Data
      {
        if (DEBUG)
          Serial.println(readdata_err);
        return 0;
      }
    }
    else
    {
      if (DEBUG)
        Serial.println(ack_err);
      return 0;
    }

    //Check SAMConfiguration response
    if ((ReadData[5] != 0xD5) || (ReadData[6] != 0x15))
    {
      if (DEBUG)
        Serial.println(data_err);  
      return 0;
    }

    return 1;
}

uint8_t PN532_GetFirmwareVersion()
{
    uint8_t WriteData[1];
    uint8_t ReadData[13];

    //Send Command
    WriteData[0] = 0x02;
    writecommand(WriteData, 1);
    //Check Ack
    if(checkAck())
    { 
      //Read Data
      if(!readdata(ReadData,6+7,TIMEOUT)) //Read Data
      {
        if (DEBUG)
          Serial.println(readdata_err);
        return 0;
      }
    }
    else
    {
      if (DEBUG)
        Serial.println(ack_err);
      return 0;
    }

    //Check GetFirmwareVersion response
    if ((ReadData[5] != 0xD5) || (ReadData[6] != 0x03))
    {
      if (DEBUG)
        Serial.println(data_err);  
      return 0;
    }

    Serial.print("IC      : ");
    Serial.println(ReadData[7], HEX);
    Serial.print("Ver     : ");
    Serial.println(ReadData[8], HEX);
    Serial.print("Rev     : ");
    Serial.println(ReadData[9], HEX);
    Serial.print("Support : ");
    Serial.println(ReadData[10], HEX);

    return 1;

}

uint8_t PN532_RFconfiguration()
{
    uint8_t WriteData[5];
    uint8_t ReadData[9];

    WriteData[0] = 0x32;
    WriteData[1] = 0x05; ///MaxRetries
    WriteData[2] = 0x00;
    WriteData[3] = 0x01;
    WriteData[4] = 0x02;

    //Send Command
    writecommand(WriteData, 5);
    //Check Ack
    if(checkAck())
    { 
      //Read Data
      if(!readdata(ReadData,2+7,TIMEOUT)) //Read Data
      {
        if (DEBUG)
          Serial.println(readdata_err);
        return 0;
      }
    }
    else
    {
      if (DEBUG)
        Serial.println(ack_err);
      return 0;
    }

    //Check RFconfiguration response
    if ((ReadData[5] != 0xD5) || (ReadData[6] != 0x33))
    {
      if (DEBUG)
        Serial.println(data_err);  
      return 0;
    }

    return 1;
}

uint8_t PN532_InlistPassiveTarget()
{
    uint8_t WriteData[4];
    uint8_t ReadData[10];

    WriteData[0] = 0x4a;
    WriteData[1] = 0x01;
    WriteData[2] = 0x03;
    WriteData[3] = 0x00;

    //Send Command
    writecommand(WriteData, 4);
    //Check Ack
    if(checkAck())
    { 
      //Read Data
      if(!readdata(ReadData,3+7,TIMEOUT)) //Read Data
      {
        if (DEBUG)
          Serial.println(readdata_err);
        return 0;
      }
    }
    else
    {
      if (DEBUG)
        Serial.println(ack_err);
      return 0;
    }

    //Check InlistPassiveTarget response
    if ((ReadData[5] != 0xD5) || (ReadData[6] != 0x4B))
    {
      if (DEBUG)
        Serial.println(data_err);  
      return 0;
    }
       
    return 1;
}

uint8_t PN532_Initiate(uint8_t* chip_id)
{
    uint8_t WriteData[3];
    uint8_t ReadData[11];

    ///InCommunicateThru
    WriteData[0] = 0x42;
    WriteData[1] = 0x06;
    WriteData[2] = 0x00;
    //Send Command
    writecommand(WriteData, 3);
    //Check Ack
    if(checkAck())
    { 
      //Read Data
      if(!readdata(ReadData,4+7,TIMEOUT)) //Read Data
      {
        if (DEBUG)
          Serial.println(readdata_err);
        return 0;
      }
    }
    else
    {
      if (DEBUG)
        Serial.println(ack_err);
      return 0;
    }

    //Check Initiate response
    if ((ReadData[5] != 0xD5) || (ReadData[6] != 0x43) || (ReadData[7] != 0x00)) //7-Status
    {
      if (DEBUG)
        Serial.println(data_err);  
      return 0;
    }

    *chip_id = ReadData[8];

    return 1;
}

uint8_t PN532_Select(uint8_t chip_id)
{
    uint8_t WriteData[3];
    uint8_t ReadData[11];

    ///InCommunicateThru
    WriteData[0] = 0x42;
    WriteData[1] = 0x0e;
    WriteData[2] = chip_id;
    //Send Command
    writecommand(WriteData, 3);
    //Check Ack
    if(checkAck())
    { 
      //Read Data
      if(!readdata(ReadData,4+7,TIMEOUT)) //Read Data
      {
        if (DEBUG)
          Serial.println(readdata_err);
        return 0;
      }
    }
    else
    {
      if (DEBUG)
        Serial.println(ack_err);
      return 0;
    }

    //Check Select response
    if ((ReadData[5] != 0xD5) || (ReadData[6] != 0x43) || (ReadData[7] != 0x00) || (ReadData[8] != chip_id)) //7-Status 8-Chip ID
    {
      if (DEBUG)
        Serial.println(data_err);  
      return 0;
    }

    return 1;
}

uint8_t PN532_ReadBlock(uint8_t address, uint8_t *readBlock)
{
    uint8_t WriteData[3];
    uint8_t ReadData[14];

    ///Invio del comando InCommunicateThru
    ///Scrittura del comando READ_BLOCK 0x08 Address
    WriteData[0] = 0x42;
    WriteData[1] = 0x08;
    WriteData[2] = address;
    //Send Command
    writecommand(WriteData, 3);
    //Check Ack
    if(checkAck())
    { 
      //Read Data
      if(!readdata(ReadData,7+7,TIMEOUT)) //Read Data
      {
        if (DEBUG)
          Serial.println(readdata_err);
        return 0;
      }
    }
    else
    {
      if (DEBUG)
        Serial.println(ack_err);
      return 0;
    }

    //Check ReadBlock response
    if ((ReadData[5] != 0xD5) || (ReadData[6] != 0x43) || (ReadData[7] != 0x00)) //7-Status 
    {
      if (DEBUG)
        Serial.println(data_err);  
      return 0;
    }

    readBlock[0] = ReadData[8];
    readBlock[1] = ReadData[9];
    readBlock[2] = ReadData[10];
    readBlock[3] = ReadData[11];

    return 1;
}

uint8_t PN532_WriteBlock(uint8_t address, uint8_t *writeBlock)
{
    uint8_t WriteData[3];
    uint8_t ReadData[10];

    ///Invio del comando InCommunicateThru
    ///Scrittura del comando READ_BLOCK 0x08 Address
    WriteData[0] = 0x42;
    WriteData[1] = 0x09;
    WriteData[2] = address;
    WriteData[3] = writeBlock[0];
    WriteData[4] = writeBlock[1];
    WriteData[5] = writeBlock[2];
    WriteData[6] = writeBlock[3];
    //Send Command
    writecommand(WriteData, 7);
    //Check Ack
    if(checkAck())
    { 
      //Read Data
      if(!readdata(ReadData,3+7,TIMEOUT)) //Read Data
      {
        if (DEBUG)
          Serial.println(readdata_err);
        return 0;
      }
    }
    else
    {
      if (DEBUG)
        Serial.println(ack_err);
      return 0;
    }

    //Check WriteBlock response
    if ((ReadData[5] != 0xD5) || (ReadData[6] != 0x43) || (ReadData[7] != 0x01)) //7-Status
    {
      if (DEBUG)
        Serial.println(data_err);  
      return 0;
    }

    return 1;
}

uint8_t PN532_GetUID(uint8_t *UID)
{
    uint8_t WriteData[2];
    uint8_t ReadData[18];

    ///Invio del comando InCommunicateThru
    ///Scrittura del comando GET_UID 0x0b Address
    WriteData[0] = 0x42;
    WriteData[1] = 0x0b;
    //Send Command
    writecommand(WriteData, 2);
    //Check Ack
    if(checkAck())
    { 
      //Read Data
      if(!readdata(ReadData,11+7,TIMEOUT)) //Read Data
      {
        if (DEBUG)
          Serial.println(readdata_err);
        return 0;
      }
    }
    else
    {
      if (DEBUG)
        Serial.println(ack_err);
      return 0;
    }

    //Check Initiate response
    if ((ReadData[5] != 0xD5) || (ReadData[6] != 0x43) || (ReadData[7] != 0x00)) //7-Status 
    {
      if (DEBUG)
        Serial.println(data_err);  
      return 0;
    }

    UID[0] = ReadData[8];
    UID[1] = ReadData[9];
    UID[2] = ReadData[10];
    UID[3] = ReadData[11];
    UID[4] = ReadData[12];
    UID[5] = ReadData[13];
    UID[6] = ReadData[14];
    UID[7] = ReadData[15];
    UID[8] = ReadData[16];

    return 1;
}

uint8_t checkAck()
{
  uint8_t ReadAck[7];
  
  readdata(ReadAck,6,10); //Ack
  
  if ((ReadAck[0] == 0x00) && (ReadAck[1] == 0x00) && (ReadAck[2] == 0xFF) && (ReadAck[3] == 0x00) && (ReadAck[4] == 0xFF) && (ReadAck[5] == 0x00))  
    return 1;
  else
    return 0;
}

void writecommand(uint8_t* cmd, uint8_t cmdlen, uint8_t wake=0)
{
  uint8_t checksum;

  checksum = 0xD4;

  if(!use_spi)
  {
    //I2C START
    Wire.beginTransmission(PN532_ADDRESS); 
    //PREAMBLE
    Wire.write(0x00);
    //START CODE
    Wire.write(0x00);
    Wire.write(0xFF);
    //LEN
    Wire.write(cmdlen + 1);
    //LCS
    Wire.write(255 - cmdlen);
    //TFI
    Wire.write(0xD4);
    //DATA
    for (uint8_t i=0; i<cmdlen; i++) 
    {
      Wire.write(cmd[i]);
      checksum += cmd[i];
    }
    //DCS
    Wire.write(256 - checksum);
    //POSTAMBLE
    Wire.write(0x00);
    // I2C STOP 
    Wire.endTransmission(); 
  }
  else
  { 
    SPI.beginTransaction(PN532_SPI_SETTING);
    digitalWrite(_ss, LOW);
    if (wake)    
      _delay_ms(2);
    //Data writing
    spi_write(0x01); 
    //PREAMBLE
    spi_write(0x00);
    //START CODE
    spi_write(0x00);
    spi_write(0xFF);
    //LEN
    spi_write(cmdlen + 1);
    //LCS
    spi_write(255 - cmdlen);
    //TFI
    spi_write(0xD4);
    //DATA
    for (uint8_t i=0; i<cmdlen; i++) 
    {
      spi_write(cmd[i]);
      checksum += cmd[i];
    }
    //DCS
    spi_write(256 - checksum);
    //POSTAMBLE
    spi_write(0x00);
    //END
    digitalWrite(_ss, HIGH);
  }

  if (DEBUG)
  {
    Serial.print("TX_DATA : 00 00 FF "); 
    Serial.print((cmdlen + 1),HEX);
    Serial.print(" "); 
    Serial.print((255 - cmdlen),HEX);
    Serial.print(" "); 
    Serial.print(0xD4,HEX);
    Serial.print(" "); 
    for (uint8_t i=0; i<cmdlen; i++) 
    {
      Serial.print(cmd[i],HEX);
      Serial.print(" "); 
    }
    Serial.print((256 - checksum),HEX);
    Serial.print(" "); 
    Serial.println("00"); 
  }
}

uint8_t readdata(uint8_t* buff, uint8_t cmdlen, uint8_t retry)
{  
  uint8_t RDY=0;
  uint8_t bf_read=0;
  int i;
  
  if (!use_spi)
  {
    //Read the STATUS byte
    for(i=0;i<retry;i++)
    {
      Wire.requestFrom(PN532_ADDRESS, 1, false); //Keep reading without sending STOP
      if(Wire.available() == 1)
      {
        RDY = Wire.read() & 0x01;
        if (RDY)
          break; //Break if device is ready
      }
      _delay_us(500);
    }

    if (!RDY)
    {
      //This is needed to "close" the previous requestFrom
      Wire.requestFrom(PN532_ADDRESS, 1);
      return 0;
    }

    //Start to read data
    Wire.requestFrom(PN532_ADDRESS, cmdlen + 1);
    if(Wire.available()==(cmdlen + 1))
    {
      for (i=0; i<(cmdlen+1); i++)
      {
        bf_read = Wire.read();
        if (i>0) //Don't save STATUS byte
          buff[i-1] = bf_read;
      }

      if (DEBUG)
      {
        Serial.print("RX_DATA : ");
        for (i=0; i<cmdlen; i++)
        {
          Serial.print(buff[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
      }

    }
    else
      return 0;

    return 1;
  }
  else
  {
    uint8_t ready_b;

    for(i=0;i<retry;i++)
    {
      //Read the READY byte
      SPI.beginTransaction(PN532_SPI_SETTING);
      digitalWrite(_ss, LOW);
      //Status reading
      spi_write(0x02);
      //Read data
      ready_b = spi_read();
      //END
      digitalWrite(_ss, HIGH);

      if(ready_b)
        break;
      
      _delay_us(2000);
    }

    if (!ready_b)
      return 0;
      
    //Read the whole data
    SPI.beginTransaction(PN532_SPI_SETTING);
    digitalWrite(_ss, LOW);
    //_delay_us(2000);
    //Data read
    spi_write(0x03);
    for (i=0; i<cmdlen; i++)
      buff[i] = spi_read();
    //END
    digitalWrite(_ss, HIGH);

    if (DEBUG)
    {
      Serial.print("RX_DATA : ");
      for (i=0; i<cmdlen; i++)
      {
        Serial.print(buff[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }

    return 1;
  }    
}

void spi_write(uint8_t c) {
    //TBD: support for software SPI
    SPI.transfer(c);
}

uint8_t spi_read(void) {
  int8_t x=0;

  //TBD: support for software SPI
  x = SPI.transfer(0x00);
  
  return x;
}
