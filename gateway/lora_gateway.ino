#include <Bridge.h>
#include <Mailbox.h>
#include <HttpClient.h>
#include <SPI.h>
#include <RH_RF95.h>

#define LOCK_0 0x37
#define LOCK_1 0x73
#define GATEWAY 0x77

RH_RF95 rf95;
float frequency = 915.0;
String server_url = "http://192.168.43.160:8080/from_lora";
String lock_param = "?lock=";
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

  rf95.setThisAddress(GATEWAY);
  rf95.setHeaderFrom(GATEWAY);

  Console.println("Gateway Ready");
}

void loop() {
  uint8_t lora_out[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t lora_in[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(lora_in);
  String message;
  HttpClient client;
  //uint8_t lora_in[] = {0x0f, 0x88, 0x6e, 0x09, 0x5e, 0x89, 0xcf, 0x95};

  Console.println("Checking for LoRa comm");
  if (rf95.waitAvailableTimeout(100)){
    if (rf95.recv(lora_in, &len)){
      Console.println("LoRa comm received: ");
      uint8_t origin = rf95.headerFrom();
      if (origin == LOCK_0 || origin == LOCK_1){
        
        Console.print("Origin: Lock ");
        Console.println(origin, HEX);
        // Converts bytes to String
        String in_str = "";
        char temp[3];
        for (int i = 0; i < 8; ++i){
          itoa(lora_in[i + 3], temp, 16); // i + 3 for header offset
          in_str += temp;
        }
        String origin_str = "";
        itoa(origin, temp, 16);
        origin_str += temp;
        Console.print("MSG: ");
        Console.println(in_str);
        // send to python server
        Console.println("Sending message to server");
        String url = server_url;
        url += lock_param + origin_str;
        url += msg_param + in_str;
        client.get(url);
        Console.println("Message sent");
      } else {
        Console.println("Unknown origin, ignoring");
      }
      
    } else {
      Console.println("Received invalid comm");
    }
  }

  Console.println("Checking mailbox");
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

      uint8_t dest = strtoul(message.substring(0,2).c_str(), NULL, 16);

      // Converts String to bytes
      int idx;
      for (int i = 0; i < 8; ++i){
        idx = 2 * (i + 1);
        lora_out[i] = strtoul(message.substring(idx, idx + 2).c_str() , NULL, 16);
      }
      
      for (int i = 0; i < 8; ++i){
        Console.print("0x");
        Console.print(lora_out[i], HEX);
        Console.print(" ");
      }
      Console.println("");

      rf95.setHeaderTo(dest);
      rf95.send(lora_out, sizeof(lora_out));
      rf95.waitPacketSent();
    }
  }

  Console.println("Cycle complete");
  delay(100);
}
