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
#define PN532_COMMAND_RF_CONFIG (0x32)
#define PN532_COMMAND_INLISTPASSIVETARGET (0x4A)
#define PN532_COMMAND_SAMCONFIG (0x14)

// Prototypes
uint8_t nfc_get_response_size();

uint8_t nfc_read_ack();

uint8_t nfc_read_response();

uint8_t *nfc_get_response_buffer();

void nfc_init(void);

void nfc_send_command(const uint8_t command, const uint8_t info_buf[], const uint8_t length);

uint8_t nfc_GetGeneralStatus();

uint8_t nfc_InListPassiveTarget();

uint8_t nfc_SAMConfig();

uint8_t nfc_GetFirmwareVersion();

uint8_t nfc_is_interrupt_handled(void);

void nfc_set_interrupt_handled(uint8_t val);

uint8_t nfc_last_was_ack();
#endif /* NFC_H_ */
