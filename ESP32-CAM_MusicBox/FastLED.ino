#define LED_PIN     12
#define NUM_LEDS    15
#define BRIGHTNESS  12
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 RammsteinColors_p PROGMEM;
extern const TProgmemPalette16 Battery_Charging_p PROGMEM;
extern const TProgmemPalette16 Battery_Charged_p PROGMEM;
extern const TProgmemPalette16 Battery_Empty_p PROGMEM;


void setup_FastLED() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  fill_solid( currentPalette, 16, CRGB::White);
  FastLED.setBrightness( BRIGHTNESS );

  FastLEDColor(CRGB::White);

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}

void loop_FastLED() {
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors(startIndex);

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FastLEDColor( const CRGB& rgb)
{
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = rgb;
  }
  FastLED.show();
}

void FillLEDsFromPaletteColors( uint8_t colorIndex) {
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void SelectPalette(int Palette) {

  switch (Palette) {
    case 1:
      currentPalette = CloudColors_p;
      currentBlending = LINEARBLEND;
      break;
    case 2:
      currentPalette = LavaColors_p;
      currentBlending = NOBLEND;
      break;
    case 3:
      currentPalette = OceanColors_p;
      currentBlending = LINEARBLEND;
      break;
    case 4:
      currentPalette = ForestColors_p;
      currentBlending = NOBLEND;
      break;
    case 5:
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
      break;
    case 6:
      currentPalette = RainbowStripeColors_p;
      currentBlending = NOBLEND;
      break;
    case 7:
      currentPalette = HeatColors_p;
      currentBlending = LINEARBLEND;
      break;
    case 8:
      currentPalette = LavaColors_p;
      currentBlending = LINEARBLEND;
      break;
    case 9:
      currentPalette = RainbowStripeColors_p;
      currentBlending = LINEARBLEND;
      break;
    case 10:
      currentPalette = ForestColors_p;
      currentBlending = LINEARBLEND;
      break;
    case 80:
      currentPalette = RammsteinColors_p;
      currentBlending = NOBLEND;
      break;
    default:
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
      break;
  }
}

void SelectChargingPalette(int State) {
  switch (State) {
    case 1:
      currentPalette = Battery_Charged_p;
      currentBlending = NOBLEND;
      break;
    case 2:
      currentPalette = Battery_Charging_p;
      currentBlending = NOBLEND;
      break;
    case 3:
      currentPalette = Battery_Empty_p;
      currentBlending = NOBLEND;
      break;
    default:
      break;
  }
}

const TProgmemPalette16 RammsteinColors_p PROGMEM =
{
  CRGB::Black,
  CRGB::White,
  CRGB::White,
  CRGB::Black,
  CRGB::Black,
  CRGB::White,
  CRGB::White,
  CRGB::Black,
  CRGB::Black,
  CRGB::White,
  CRGB::White,
  CRGB::Black,
  CRGB::Black,
  CRGB::White,
  CRGB::White,
  CRGB::Black
};

const TProgmemPalette16 Battery_Charging_p PROGMEM =
{
  CRGB::Yellow,
  CRGB::White,
  CRGB::Yellow,
  CRGB::White,
  CRGB::Yellow,
  CRGB::White,
  CRGB::Yellow,
  CRGB::White,
  CRGB::Yellow,
  CRGB::White,
  CRGB::Yellow,
  CRGB::White,
  CRGB::Yellow,
  CRGB::White,
  CRGB::Yellow,
  CRGB::White
};

const TProgmemPalette16 Battery_Charged_p PROGMEM =
{
  CRGB::Yellow,
  CRGB::White,
  CRGB::Yellow,
  CRGB::White,
  CRGB::Yellow,
  CRGB::White,
  CRGB::Yellow,
  CRGB::White,
  CRGB::Yellow,
  CRGB::White,
  CRGB::Yellow,
  CRGB::White,
  CRGB::Yellow,
  CRGB::White,
  CRGB::Yellow,
  CRGB::White
};


const TProgmemPalette16 Battery_Empty_p PROGMEM =
{
  CRGB::Red,
  CRGB::White,
  CRGB::Red,
  CRGB::White,
  CRGB::Red,
  CRGB::White,
  CRGB::Red,
  CRGB::White,
  CRGB::Red,
  CRGB::White,
  CRGB::Red,
  CRGB::White,
  CRGB::Red,
  CRGB::White,
  CRGB::Red,
  CRGB::White
};
