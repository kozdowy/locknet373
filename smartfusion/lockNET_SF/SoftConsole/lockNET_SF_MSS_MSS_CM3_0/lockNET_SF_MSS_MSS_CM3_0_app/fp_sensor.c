#include "fp_sensor.h"

uint8_t id_enrolled[20];
uint32_t baud_rate = 9600;

uint8_t FP_init(void){
  MSS_UART_init(
                &g_mss_uart0,
                MSS_UART_9600_BAUD,
                MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT
                );
  int i = 0;
  for (i; i < 20; ++i){
    id_enrolled[i] = (FP_check_enrolled(i).command == FP_ACK);
  }
  //send initialization - command = open, parameter = 0, get ack back
  comm_packet res = FP_init_device();
  return res.command == FP_ACK;
}

uint8_t FP_get_next_empty_id(void){
  int i = 0;
  for (i = 0; i < 20; ++i){
    if (id_enrolled[i]){
      return i;
    }
  }
  return FP_IDS_FULL;
}

uint32_t FP_enroll(void){
  uint8_t id = FP_get_next_empty_id();
  //enrollstart (with not used ID)
  comm_packet res = FP_enroll_start(id);
  if (res.command == FP_NACK){
    return res.parameter;
  }
  //capturefinger
  res = FP_capture_finger(1); // use quality
  while (res.command == FP_NACK &&
         res.parameter == FP_NACK_FINGER_IS_NOT_PRESSED){
    res = FP_capture_finger(1);
  }
  //enroll1
  res = FP_enroll_1();
  if (res.command == FP_NACK){
    return res.parameter;
  }
  //wait to take off finger using ispressfinger
  while (!FP_n_finger_pressed().parameter);
  //capturefinger
  res = FP_capture_finger(1); // use quality
  while (res.command == FP_NACK &&
         res.parameter == FP_NACK_FINGER_IS_NOT_PRESSED){
    res = FP_capture_finger(1);
  }
  //enroll2
  res = FP_enroll_2();
  if (res.command == FP_NACK){
    return res.parameter;
  }
  //wait to take off finger using ispressfinger
  while (!FP_n_finger_pressed().parameter);
  //capturefinger
  res = FP_capture_finger(1); // use quality
  while (res.command == FP_NACK &&
         res.parameter == FP_NACK_FINGER_IS_NOT_PRESSED){
    res = FP_capture_finger(1);
  }
  //enroll3
  res = FP_enroll_3();
  if (res.command == FP_NACK){
    return res.parameter;
  }

  enrolled_ids[i] = 1;
  return 0;
}

uint32_t FP_read_and_verify(void){
  //capturefinger
  comm_packet res = FP_capture_finger(1); // use quality
  while (res.command == FP_NACK &&
         res.parameter == FP_NACK_FINGER_IS_NOT_PRESSED){
    res = FP_capture_finger(1);
  }
  //identify
  res = FP_identify_capture();
  return res.parameter;
}

comm_packet FP_init_device(void){
  return FP_send_command(FP_OPEN, 0);
}

comm_packet FP_cmos_led(uint8_t led_state){
  return FP_send_command(FP_CMOS_LED, led_state);
}

comm_packet FP_change_baud_rate(uint32_t baud_rate){
  return FP_send_command(FP_CHANGE_BAUDRATE, baud_rate);
}

comm_packet FP_get_enroll_count(void){
  return FP_send_command(FP_GET_ENROLL_COUNT, NO_PARAMETER);
}

comm_packet FP_check_enrolled(uint8_t id){
  return FP_send_command(FP_CHECK_ENROLLED, id);
}

comm_packet FP_enroll_start(uint8_t id){
  return FP_send_command(FP_ENROLL_START, id);
}

comm_packet FP_enroll_1(void){
  return FP_send_command(FP_ENROLL_1, NO_PARAMETER);
}

comm_packet FP_enroll_2(void){
  return FP_send_command(FP_ENROLL_2, NO_PARAMETER);
}

comm_packet FP_enroll_3(void){
  return FP_send_command(FP_ENROLL_3, NO_PARAMETER);
}

comm_packet FP_n_finger_pressed(void){
  return FP_send_command(FP_FINGER_PRESSED, NO_PARAMETER);
}

comm_packet FP_delete_ID(uint8_t id){
  return FP_send_command(FP_DELETE_ID, id);
}

comm_packet FP_delete_all(void){
  return FP_send_command(FP_DELETE_ALL);
}

comm_packet FP_verify_capture(uint8_t id){
  return FP_send_command(FP_VERIFY_CAPTURE, id);
}

comm_packet FP_identify_capture(uint8_t id){
  return FP_send_command(FP_IDENTIFY_CAPTURE, id);
}

comm_packet FP_verify_template(uint8_t id, uint8_t* template){
  comm_packet res = FP_send_command(FP_VERIFY_TEMPLATE, id);
  if (res.command == FP_ACK){
    return FP_send_data(template, FP_TEMPLATE_LENGTH);
  }
  return res;
}

comm_packet FP_identify_template(uint8_t* template){
  comm_packet res = FP_send_command(FP_IDENTIFY_TEMPLATE, NO_PARAMETER);
  if (res.command == FP_ACK){
    return FP_send_data(template, FP_TEMPLATE_LENGTH);
  }
  return res;
}

comm_packet FP_capture_finger(uint8_t quality){
  return FP_send_command(FP_CAPTURE_FINGER, quality);
}

comm_packet FP_make_template(uint8_t* template){
  comm_packet res = FP_send_command(FP_MAKE_TEMPLATE, NO_PARAMETER);
  if (res.command == FP_ACK){
    return FP_recv_data(template, FP_TEMPLATE_LENGTH);
  }
  return res;
}

comm_packet FP_get_image(uint8_t* img){
  comm_packet res = FP_send_command(FP_GET_IMAGE, NO_PARAMETER);
  if (res.command == FP_ACK){
    return FP_recv_data(img, FP_IMAGE_LENGTH);
  }
  return res;
}

comm_packet FP_get_raw_image(uint8_t* img){
  comm_packet res = FP_send_command(FP_GET_RAW_IMAGE, NO_PARAMETER);
  if (res.command == FP_ACK){
    return FP_recv_data(img, FP_IMAGE_LENGTH)
      }
}

comm_packet FP_get_template(uint8_t id, uint8_t* template){
  comm_packet res = FP_send_command(FP_GET_TEMPLATE, id);
  if (res.command == FP_ACK){
    return FP_recv_data(template, FP_TEMPLATE_LENGTH);
  }
  return res;
}

comm_packet FP_set_template(uint8_t id, uint8_t* template){
  comm_packet res = FP_send_command(FP_SET_TEMPLATE, id);
  if (res.command == FP_ACK){
    return FP_send_data(template, FP_TEMPLATE_LENGTH);
  }
  return res;
}

comm_packet FP_send_command(uint8_t command, uint32_t parameter){
  return 0;
}

comm_packet FP_recv_data(uint8_t* data_buf, uint32_t data_len){
  return 0;
}

comm_packet FP_send_data(uint8_t* data_buf, uint32_t data_len){
  return 0;
}
