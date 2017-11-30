/*
 * fp_sensor.h
 *
 *  Created on: Nov 21, 2017
 *      Author: kozdowy
 */

#ifndef FP_SENSOR_H_
#define FP_SENSOR_H_

#include <stdint.h>
#include "drivers/mss_uart.h"

#define FP_COMMAND_START_1 0x55
#define FP_COMMAND_START_2 0xAA
#define FP_RESPONSE_START_1 0x55
#define FP_RESPONSE_START_2 0xAA
#define FP_DATA_START_1 0x5A
#define FP_DATA_START_2 0xA5
#define FP_DEFAULT_DEVICE_ID 0x0001

#define FP_OPEN 0x01
#define FP_CLOSE 0x02
#define FP_USB_CHECK 0x03 // UsbInternalCheck
#define FP_CHANGE_BAUDRATE 0x04
#define FP_CMOS_LED 0x12
#define FP_GET_ENROLL_COUNT 0x20
#define FP_CHECK_ENROLLED 0x21
#define FP_ENROLL_START 0x22
#define FP_ENROLL_1 0x23
#define FP_ENROLL_2 0x24
#define FP_ENROLL_3 0x25
#define FP_FINGER_PRESSED 0x26 // IsPressFinger
#define FP_DELETE_ID 0x40
#define FP_DELETE_ALL 0x41
#define FP_VERIFY_CAPTURE 0x50 // Verify
#define FP_IDENTIFY_CAPTURE 0x51 // Identify
#define FP_VERIFY_TEMPLATE 0x52
#define FP_IDENTIFY_TEMPLATE 0x53
#define FP_CAPTURE_FINGER 0x60
#define FP_MAKE_TEMPLATE 0x61
#define FP_GET_IMAGE 0x62
#define FP_GET_RAW_IMAGE 0x63
#define FP_GET_TEMPLATE 0x70
#define FP_SET_TEMPLATE 0x71

#define FP_ACK 0x30
#define FP_NACK 0x31

#define FP_NACK_INVALID_POS 0x1003 // specified ID not between 0-19
#define FP_NACK_IS_NOT_USED 0x1004 // specified ID not used
#define FP_NACK_IS_ALREADY_USED 0x1005 // specified ID already used
#define FP_NACK_COMM_ERR 0x1006 // communication error
#define FP_NACK_VERIFY_FAILED 0x1007 // 1:1 verification failure
#define FP_NACK_IDENTIFY_FAILED 0x1008 // 1:N identification failure
#define FP_NACK_DB_IS_FULL 0x1009 // database is full
#define FP_NACK_DB_IS_EMPTY 0x100A // database is empty
#define FP_NACK_BAD_FINGER 0x100C // bad fingerprint
#define FP_NACK_ENROLL_FAILED 0x100D // enrollment failure
#define FP_NACK_IS_NOT_SUPPORTED 0x100E // command is not supported
#define FP_NACK_DEV_ERR 0x100F // device error
#define FP_NACK_INVALID_PARAM 0x1011 // invalid parameter
#define FP_NACK_FINGER_IS_NOT_PRESSED 0x1012 // finger is not pressed

#define FP_TEMPLATE_LENGTH 506
#define FP_IMAGE_LENGTH 51840

#define FP_IDS_FULL 0xff

typedef struct comm_packet{
  uint32_t parameter;
  uint16_t command;
};

/*
 * initializes uart and device
 */
uint8_t FP_init(void);

/*
 * finds next unused id in device
 */
uint8_t FP_get_next_empty_id(void);

/*
 * enrolls a new finger in next available location
 */
uint32_t FP_enroll(void);

/*
 * reads in a fingerprint, then verifies if it is enrolled
 * RETURNS: FP_ACK if ok, (FP_identify_capture param) if not

/*
 * COMMAND PACKET
 * Command = Open
 * Parameter = 0: not get extra info, nonzero: get extra info
 * RESPONSE PACKET
 * Response = Ack
 * If "get extra info":
 * DATA PACKET
 * DWORD FirmwareVersion
 * DWORD IsoAreaMaxSize
 * BYTE DeviceSerialNumber[16]
 */
comm_packet FP_init_device(void);

/*
 * default state is off
 * turn led on prior to any capture
 * 
 * COMMAND PACKET
 * Command = CmosLed
 * Parameter = 0: Off, Nonzero: On
 * RESPONSE PACKET
 * Response = Ack
 */
comm_packet FP_cmos_led(uint8_t led_state);

/*
 * default baud rate is 9600
 *
 * COMMAND PACKET
 * Command = ChangeBaudRate
 * Parameter = 9600-115200 (baud rate)
 * RESPONSE PACKET
 * Response = Ack OR Nack: Error NACK_INVALID_PARAM
 */
comm_packet FP_change_baud_rate(uint32_t baud_rate);

/*
 * COMMAND PACKET
 * Command = GetEnrollCount
 * RESPONSE PACKET
 * Response = Ack
 * Parameter = Enrolled Fingerprint Count
 */
comm_packet FP_get_enroll_count(void);

/*
 * COMMAND PACKET
 * Command = CheckEnrolled
 * Parameter = 0-19 (ID)
 * RESPONSE PACKET
 * If enrolled
 * Response = Ack
 * If not enrolled
 * Response = Nack: Error NACK_INVALID_POS, NACK_IS_NOT_USED
 */
comm_packet FP_check_enrolled(uint8_t id);

/*
 * COMMAND PACKET
 * Command = EnrollStart
 * Parameter = 0-19 (ID)
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack:
 * Error NACK_DB_IS_FULL, NACK_INVALID_POS, NACK_IS_ALREADY_USED
 */
comm_packet FP_enroll_start(uint8_t id);

/*
 * COMMAND PACKET
 * Command = Enroll1
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack
 * Error NACK_ENROLL_FAILED, NACK_BAD_FINGER
 */
comm_packet FP_enroll_1(void);

/*
 * COMMAND PACKET
 * Command = Enroll2
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack
 * Error NACK_ENROLL_FAILED, NACK_BAD_FINGER
 */
comm_packet FP_enroll_2(void);

/*
 * COMMAND PACKET
 * Command = Enroll3
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack
 * Error NACK_ENROLL_FAILED, NACK_BAD_FINGER
 */
comm_packet FP_enroll_3(void);

/*
 * COMMAND PACKET
 * Command = IsPressFinger
 * RESPONSE PACKET
 * Response = Ack
 * Parameter = 0: pressed, nonzero: not pressed
 */
comm_packet FP_n_finger_pressed(void);

/*
 * COMMAND PACKET
 * Command = DeleteID
 * Parameter = 0-19 (ID)
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack
 * Error NACK_INVALID_POS
 */
comm_packet FP_delete_ID(uint8_t id);

/*
 * COMMAND PACKET
 * Command = DeleteAll
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack
 * Error NACK_DB_IS_EMPTY
 */
comm_packet FP_delete_all(void);

/*
 * COMMAND PACKET
 * Command = Verify
 * Parameter = 0-19 (ID)
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack
 * Error NACK_INVALID_POS, NACK_IS_NOT_USED, NACK_VERIFY_FAILED
 */
comm_packet FP_verify_capture(uint8_t id);

/*
 * COMMAND PACKET
 * Command = Identify
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * Parameter = 0-19 (identified ID)
 * If not OK
 * Response = Nack
 * Error NACK_DB_IS_EMPTY, NACK_IDENTIFY_FAILED
 */
comm_packet FP_identify_capture(uint8_t id);

/*
 * COMMAND PACKET
 * Command = VerifyTemplate
 * Parameter = 0-19 (ID)
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack
 * Error NACK_INVALID_POS, NACK_IS_NOT_USED
 * DATA PACKET
 * Data = template (506 bytes)
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack
 * Error NACK_COMM_ERR, NACK_VERIFY_FAILED
 */
comm_packet FP_verify_template(uint8_t id, uint8_t* template);

/*
 * COMMAND PACKET
 * Command = IdentifyTemplate
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack
 * Error NACK_DB_IS_EMPTY
 * DATA PACKET
 * Data = template (506 bytes)
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * Parameter = 0-19 (identified ID)
 * If not OK
 * Response = Nack
 * Error NACK_COMM_ERR, NACK_IDENTIFY_FAILED
 */
comm_packet FP_identify_template(uint8_t* template);

/*
 * use best image for enrollment
 * use not best image for identification/verification
 *
 * COMMAND PACKET
 * Command = CaptureFinger
 * Parameter = 0: not best but fast, nonzero: best but slow
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack
 * Error = NACK_FINGER_IS_NOT_PRESSED
 */
comm_packet FP_capture_finger(uint8_t quality);

/*
 * COMMAND PACKET
 * Command = MakeTemplate
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack
 * Error = NACK_BAD_FINGER
 * DATA PACKET
 * Data = made template (506 bytes)
 */
comm_packet FP_make_template(uint8_t* template);

/*
 * COMMAND PACKET
 * Command = GetImage
 * RESPONSE PACKET
 * Response = Ack
 * DATA PACKET
 * Data = 240x216 image (51840 bytes)
 */
comm_packet FP_get_image(uint8_t* img);

/*
 * COMMAND PACKET
 * Command = GetRawImage
 * RESPONSE PACKET
 * Response = Ack
 * DATA PACKET
 * Data = 240x216 QVGA image (51840 bytes)
 */
comm_packet FP_get_raw_image(uint8_t* img);

/*
 * COMMAND PACKET
 * Command = GetTemplate
 * Parameter = 0-19 (ID)
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack
 * Error = NACK_INVALID_POS, NACK_IS_NOT_USED
 * DATA PACKET
 * Data = template (506 bytes)
 */
comm_packet FP_get_template(uint8_t id, uint8_t* template);

/*
 * if parameter's HIWORD is non-zero
 * fingerprint duplication check will not be performed
 *
 * COMMAND PACKET
 * Command = SetTemplate
 * Parameter = 0-19 (ID)
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack
 * Error = NACK_INVALID_POS
 * DATA PACKET (to device)
 * Data = template (506 bytes)
 * RESPONSE PACKET
 * If OK
 * Response = Ack
 * If not OK
 * Response = Nack
 * Error = NACK_COMM_ERR, NACK_DEV_ERR
 */
comm_packet FP_set_template(uint8_t id, uint8_t* template);

comm_packet FP_send_command(uint16_t parameter, uint8_t command_code);

comm_packet FP_recv_data(uint8_t* data_buf, uint32_t data_len);

comm_packet FP_send_data(uint8_t* data_buf, uint32_t data_len);

#endif /* FP_SENSOR_H_ */
