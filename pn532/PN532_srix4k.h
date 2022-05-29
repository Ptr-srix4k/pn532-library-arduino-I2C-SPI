#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H

#define DEBUG 0
#define TIMEOUT 1000
#define PN532_ADDRESS (0x48 >> 1)
#define PN532_SPI_SETTING SPISettings(500000, LSBFIRST, SPI_MODE0)

#include <Arduino.h>

void PN532_use_spi(uint8_t ss);
uint8_t PN532_SAMConfiguration();
uint8_t PN532_GetFirmwareVersion();
uint8_t PN532_RFconfiguration();
uint8_t PN532_InlistPassiveTarget();
uint8_t PN532_Initiate(uint8_t* chip_id);
uint8_t PN532_Select(uint8_t chip_id);
uint8_t PN532_ReadBlock(uint8_t address, uint8_t *readBlock);
uint8_t PN532_WriteBlock(uint8_t address, uint8_t *writeBlock);
uint8_t PN532_GetUID(uint8_t *UID);
uint8_t checkAck();
void writecommand(uint8_t* cmd, uint8_t cmdlen, uint8_t wake=0);
uint8_t readdata(uint8_t* buff, uint8_t cmdlen, uint8_t retry);
void spi_write(uint8_t c);
uint8_t spi_read(void);

#endif
