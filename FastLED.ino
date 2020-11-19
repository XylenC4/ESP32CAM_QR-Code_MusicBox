#include <FastLED.h>

#define LED_PIN     12
#define NUM_LEDS    15
#define BRIGHTNESS  25
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define BUTTON_QR 13
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup_FastLED() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  fill_solid( currentPalette, 16, CRGB::White);
  FastLED.setBrightness( BRIGHTNESS );

  leds[0] = CRGB::White;
  leds[4] = CRGB::White;
  leds[5] = CRGB::White;
  leds[9] = CRGB::White;
  leds[10] = CRGB::White;
  leds[14] = CRGB::White;

  FastLED.show();

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}

void loop_FastLED() {
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors( startIndex);


  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FastLEDWhite() {
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  
  leds[0] = CRGB::White;
  leds[4] = CRGB::White;
  leds[5] = CRGB::White;
  leds[9] = CRGB::White;
  leds[10] = CRGB::White;
  leds[14] = CRGB::White;
  
  FastLED.show();
  delay(100);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex) {
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}
