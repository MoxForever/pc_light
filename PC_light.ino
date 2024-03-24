/*
config of OpenRGB at path %appdata%\OpenRGB\OpenRGB.json

"LEDStripDevices": {
    "devices": [
        {
            "baud": 115200,
            "name": "arduino nano",
            "num_leds": 144,
            "port": "COM3",  <-- set your own post
            "protocol": "adalight"
        }
    ]
},

*/

#include <FastLED.h>

#define NUM_LEDS 18
#define NUM_PINS 8

uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;
bool is_on = true, pressed = false;

CRGB leds[NUM_PINS][NUM_LEDS];

void setup() {
  pinMode(13, INPUT);

  FastLED.addLeds<WS2812, 2, GRB>(leds[0], NUM_LEDS);
  FastLED.addLeds<WS2812, 3, GRB>(leds[1], NUM_LEDS);
  FastLED.addLeds<WS2812, 4, GRB>(leds[2], NUM_LEDS);
  FastLED.addLeds<WS2812, 5, GRB>(leds[3], NUM_LEDS);
  FastLED.addLeds<WS2812, 6, GRB>(leds[4], NUM_LEDS);
  FastLED.addLeds<WS2812, 7, GRB>(leds[5], NUM_LEDS);
  FastLED.addLeds<WS2812, 8, GRB>(leds[6], NUM_LEDS);
  FastLED.addLeds<WS2812, 9, GRB>(leds[7], NUM_LEDS);
  
  LEDS.showColor(CRGB(10, 10, 10));
  delay(200);
  LEDS.showColor(CRGB(0, 0, 0));
  
  Serial.begin(115200);
  Serial.print("Ada\n");
}

void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t i) {
  leds[i / NUM_LEDS][i % NUM_LEDS].r = r * is_on;
  leds[i / NUM_LEDS][i % NUM_LEDS].g = g * is_on;
  leds[i / NUM_LEDS][i % NUM_LEDS].b = b * is_on;
}

void wait_serial() {
  while (!Serial.available()) {
    if (digitalRead(13) == LOW) {
      if (!pressed) is_on = !is_on;
      pressed = true;
    }
    else {
      pressed = false;
    }
  }
}

void loop() { 
  for(i = 0; i < sizeof prefix; ++i) {
    waitLoop: 
    wait_serial();
    if(prefix[i] == Serial.read()) continue;
    i = 0;
    goto waitLoop;
  }
  
  wait_serial();
  hi=Serial.read();
  wait_serial();
  lo=Serial.read();
  wait_serial();
  chk=Serial.read();

  if (chk != (hi ^ lo ^ 0x55))
  {
    i=0;
    goto waitLoop;
  }
  
  memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
  
  
  for (uint8_t i = 0; i < NUM_LEDS * NUM_PINS; i++) {
    byte r, g, b;
    wait_serial();
    r = Serial.read();
    wait_serial();
    g = Serial.read();
    wait_serial();
    b = Serial.read();
    set_color(r, g, b, i);
  }
  
 FastLED.show();
}
