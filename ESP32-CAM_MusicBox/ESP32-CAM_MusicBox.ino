//#define ESP32_RTOS  // Uncomment this line if you want to use the code with freertos only on the ESP32
// Has to be done before including "OTA.h"
#include "OTA.h"
#include "FastLED.h"
#include "credentials.h"


// Push button on Boot to enable WIFI without running the actual scetch (failsafe).
// Push the button again to run the scetch with enabled WIFI for debugging.
#define BUTTON_DEBUG 13
#define BUTTON_QR BUTTON_DEBUG
bool bEnableDebug = false;
bool bStartUp = false;

uint32_t entry;

void setup() {
  // Disable the Onboard LED
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW

  // Set-Up the top-lid pushbutton
  pinMode(BUTTON_DEBUG, INPUT_PULLUP);    // sets the digital pin 13 as output

  setup_FastLED();

  if (!digitalRead(BUTTON_DEBUG)) {
    bEnableDebug = true;
    bStartUp = true;
    setupOTA("ESP32-CAM_MusicBox", mySSID, myPASSWORD);
  }

  while (bStartUp) {
#ifndef defined(ESP32_RTOS) && defined(ESP32)
    TelnetStream.println(String("Upload mode active: ") + String(millis() / 1000) + String("s"));
    ArduinoOTA.handle();
#endif

    FastLEDColor(CRGB::Fuchsia);
    // Positive flank to enable the program in debug mode
    bool bButtonDebug = !digitalRead(BUTTON_DEBUG);
    static bool bButtonDebugTemp = bButtonDebug;
    if (!bButtonDebugTemp && bButtonDebug) {
      bStartUp = false;
    }
    bButtonDebugTemp = bButtonDebug;
    delay(500);
  }

  //Set-Up functions
  FastLEDColor(CRGB::Blue);
  setup_INA226();
  FastLEDColor(CRGB::Green);
  setup_DFPlayer();
  FastLEDColor(CRGB::Yellow);
  setup_QRCode();
}

void loop() {
  if (bEnableDebug) {
    entry = micros();
#ifndef defined(ESP32_RTOS) && defined(ESP32)
    ArduinoOTA.handle();
#endif

//    TelnetStream.println(micros() - entry);
//    TelnetStream.println("Loop");
  }

  //Loop functions
  loop_FastLED();
  loop_INA226();
  loop_DFPlayer();
  loop_QRCode();

  // Your code here
}
