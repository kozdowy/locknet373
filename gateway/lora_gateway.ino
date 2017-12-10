#include <Bridge.h>
#include <Mailbox.h>
#include <HttpClient.h>
#include <SPI.h>
#include <RH_RF95.h>

#define LOCK_0 0x37
#define LOCK_1 0x73

RH_RF95 rf95;
float frequency = 915.0;
String server_url = "http://192.168.43.160:8080/from_lora"
String lock_param = "?lock="
String msg_param = ";msg=";

void setup() {
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);
  // Initialize Bridge and Mailbox
  Bridge.begin(115200);
  Mailbox.begin();
  digitalWrite(A2, HIGH);

  // Initialize Serial
  Console.begin();

  // Wait until a Serial Monitor is connected.
  while (!Console);
  
  if (!rf95.init())
    Console.println("LoRa init failed");
  // Setup ISM frequency
  rf95.setFrequency(frequency);
  // Setup Power,dBm
  rf95.setTxPower(13);
  // Defaults BW Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  Console.println("Mailbox Read Message\n");
  Console.println("The Mailbox is checked every 10 seconds. The incoming messages will be shown below.\n");
}

void loop() {
  uint8_t lora_out[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t lora_in[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(lora_in);
  String message;
  HttpClient client;
  //uint8_t lora_in[] = {0x0f, 0x88, 0x6e, 0x09, 0x5e, 0x89, 0xcf, 0x95};

  if (rf95.waitAvailableTimeout(100)){
    uint8_t lock = 0;
    if (rf95.recv(lora_in, &len)){
      Console.println("LoRa comm received: ");

      if (lora_in[1] == LOCK_0){
        Console.println("Origin: lock 0");
        lock = 0;
      } else if (lora_in[1] == LOCK_1){
        Console.println("Origin: lock 1");
        lock = 1;
      } else {
        Console.println("Unknown origin");
        break;
      }
      // Converts bytes to String
      String in_str = "";
      char temp[3];
      for (int i = 0; i < 8; ++i){
        itoa(lora_in[i + 3], temp, 16); // i + 3 for header offset
        in_str += temp;
      }
      Console.print("MSG: ");
      Console.println(in_str);
      // send to python server
      Console.println("Sending message to server");
      String url = server_url;
      url += lock_param + lock;
      url += msg_param + in_str;
      client.get(url);
      Console.println("Message sent");
    }
  }

  // if there is a message in the Mailbox
  if (Mailbox.messageAvailable()) {
    Console.println("Messages available, reading now");
    // read all the messages present in the queue
    while (Mailbox.messageAvailable()) {
      Console.println("Reading in new message");
      Mailbox.readMessage(message);
      Console.println("Message read");
      Console.print(message);
      Console.println();

      // Converts String to bytes
      int idx;
      for (int i = 0; i < 8; ++i){
        idx = 2 * (i + 1);
        lora_out[i] = strtoul(message.substring(idx, idx + 2).c_str() , NULL, 16);
      }
      
      for (int i = 0; i < 8; ++i){
        Console.print("0x");
        Console.print(msg_b[i], HEX);
        Console.print(" ");
      }
      Console.println("");

      rf95.send(lora_out, sizeof(lora_out));
      rf95.waitPacketSent();
    }

    Console.println("Waiting before checking the Mailbox again");
  }

  delay(100);
}
