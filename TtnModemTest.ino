/*
  TtnModemTest
  This simple sketch connects to TTN and passes all console input to the Murata chip

  To run this script:
    - Copy arduino_secrets_distr.h to arduino_secrets.h and enter your App EUI/Key
    - Change the MKRWAN.h library file to make the 'stream' object public -- E.g.:
    From:
      262 public:
      263   typedef SerialFifo<uint8_t, LORA_RX_BUFFER> RxFifo;
      264 
      265 private:
      266   Stream&       stream;
      267   bool          network_joined;
    To:
      262 public:
      263   typedef SerialFifo<uint8_t, LORA_RX_BUFFER> RxFifo;
      264   Stream&       stream;
      265 
      266 private:        
      267   bool          network_joined;
    
*/

#include "MKRWAN.h"

LoRaModem modem;

#include "arduino_secrets.h" 
// Please enter your sensitive data in the Secret tab or arduino_secrets.h
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

void setup() {
    
  // Wait for console
  Serial.begin(115200);
  while (!Serial);

  // Initialize radio
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  }
  Serial.print("Module version is: ");
  Serial.println(modem.version());
  
  // Uncomment next line to disable ADR
  // modem.setADR(false);  

  // Connect to TTN
  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Could not connect to TTN");
    while (1) {}
  }
  Serial.println("Successfully joined TTN");

  // The following lines require custom firmware
  // Serial.print("Dev Addr: ");
  // Serial.println(modem.getDevAddr());
  // Serial.print("Nwk SKey: ");
  // Serial.println(modem.getNwkSKey());
  // Serial.print("App Skey: ");
  // Serial.println(modem.getAppSKey());

  Serial.println("Enter command mode...");
}

String buf;

void loop() {
  if (Serial.available()) {
    buf = Serial.readStringUntil('\n');
    buf.trim();
    Serial.println();
    Serial.println(buf);
    modem.stream.print(buf);
    modem.stream.print('\r');
  }
  while (modem.stream.available()) {
    Serial.write(modem.stream.read());
  }
}
