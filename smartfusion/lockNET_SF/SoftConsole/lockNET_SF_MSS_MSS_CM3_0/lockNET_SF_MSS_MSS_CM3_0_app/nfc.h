/*
 * nfc.h
 *
 *  Created on: Nov 17, 2017
 *      Author: peces
 */

#ifndef NFC_H_
#define NFC_H_

#include <stdio.h>
#include <inttypes.h>
#include "drivers/mss_i2c/mss_i2c.h"
#include <drivers/mss_gpio/mss_gpio.h>

// Target address (got it from datasheet)
#define PN532_I2C_ADDRESS (0x48>>1)
#define PN532_I2C_ADDRESS_WRITE 0x48 //DONT USE
#define PN532_I2C_ADDRESS_READ 0x49 // DONT USE

// Initialization bytes
#define PN532_PREAMBLE (0x00)
#define PN532_STARTCODE2 (0x00FF)
#define PN532_HOST_TO_NFC (0xD4)
#define PN532_NFC_TO_HOST (0xD5)
#define PN532_POSTAMBLE (0x00)

//Commands
#define PN532_COMMAND_GETFIRMWARE (0x02)
#define PN532_COMMAND_POWERDOWN (0x16)
#define PN532_COMMAND_GETSTATUS (0x04)
#define PN532_COMMAND_INAUTOPOLL (0x60)


#define PN532_POWERDOWN_INPUT (0x168801)



// Prototypes
void nfc_read(uint8_t *buff, uint8_t n);

void nfc_setup(void);

void nfc_send_command(uint8_t *command_buff, uint8_t length);

uint8_t nfc_GetGeneralStatus(uint8_t *array);



#endif /* NFC_H_ */
