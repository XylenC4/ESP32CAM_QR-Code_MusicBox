#define ESP32_RTOS  // Uncomment this line if you want to use the code with freertos only on the ESP32
// Has to be done before including "OTA.h"
#include "OTA.h"
#include <credentials.h>
#include "image_util.h"

uint32_t entry;

// Top-lid pushbutton
#define BUTTON_QR 15

void setup() {
  
  // Set-Up the top-lid pushbutton
  pinMode(BUTTON_QR, INPUT_PULLUP);    // sets the digital pin 13 as output

  // Disable the Onboard LED
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW

  // https://github.com/SensorsIot/Internet-of-Things-with-ESP8266/issues/1
  setupOTA("ESP32CAM_QR-Code_MusicBox", mySSID, myPASSWORD);

  setup_QRCode();
  setup_INA226();
  setup_FastLED();
  setup_DFPlayer();
}

void loop() {
  entry = micros();
#ifdef defined(ESP32_RTOS) && defined(ESP32)
#else // If you do not use FreeRTOS, you have to regulary call the handle method.
  ArduinoOTA.handle();
#endif
  //  TelnetStream.println(micros() - entry);
  //  TelnetStream.println("Loop");

  loop_QRCode();
  loop_INA226();
  loop_FastLED();
}
