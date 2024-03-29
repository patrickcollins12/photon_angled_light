/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "/Users/patrick/software_projects/photon_angled_light/src/angled-light.ino"
#include "Particle.h"
#include "neopixel.h"
#include "ColorC.h"

int set_rgb(String rgb);
int set_w(String w);
int set_schedule(String p);
int set_mode(String p);
int set_random_mode(String p);
int set_brightness(String b);
int set_party_speed(String s);
int set_colorcycle_speed(String s);
int set_random_mode_wait_seconds(String s);
int set_random_mode_frequency(String s);
void off(unsigned long wait );
void setup();
void loop();
void calc_rgbw();
void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength );
void colorWipe(uint32_t c, unsigned long wait);
void mixin(uint32_t c, float mix);
String printColor(uint32_t c);
void rainbow(unsigned long wait );
uint32_t WheelRGBW(byte WheelPos);
#line 5 "/Users/patrick/software_projects/photon_angled_light/src/angled-light.ino"
Rgb rgb1 = { 100,120,150};
Rgb rgb2 = { 50,120,150};
Rgb rgb3 = color_add_rgb(rgb1,rgb2,0.5);

/* ======================= prototypes =============================== */

void colorAll(uint32_t c, uint8_t wait);
void colorWipe(uint32_t c, uint8_t wait);
// void rainbow(uint8_t wait);
// void rainbow(int wait);
void rainbowCycle(uint8_t wait);
uint32_t Wheel(byte WheelPos);

/* ======================= extra-examples.cpp ======================== */

SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
// #define PIXEL_COUNT 60
#define PIXEL_COUNT 11
#define PIXEL_PIN D2
#define PIXEL_TYPE SK6812RGBW

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// set default to orange
int r_value = 255;
int g_value = 106;
int b_value = 0;
int w_value = 60;

uint32_t rgbw_value;
int brightness = 128;
int old_brightness = 0;

int party_speed = 3;
int colorcycle_speed = 1000;
int timeOutSeconds = 5;

int mix_duration = 2000;
int mix_tick= 100000;

// Turn this on to default to using the schedule in our timezone
String schedule = "on"; // on|off
int time_on = 17; // turn on  at 5pm (17:00)
int time_off=  4; // turn off at 4am (04:00)

String random_mode = "on"; // on|off

// 1s,3s,10s,360 (6m), 600s(10m)
int random_mode_wait_seconds = 120;

// 1 in 2 (50%), 1 in 4 (25%), 1 in 6 (16%), 1 in 20 (5%), 1 in 60 (1.6%)
int random_mode_frequency = 4;

// String mode = "color", "party", "natural", etc;
String mode = "natural";

// st super mode to "random" to move randomly between different modes
// String super_mode = "off";
long super_mode_last_change_millis = -1;

// String mode = "colorcycle";

// Cloud functions must return int and take one String

// unsigned long timer = 0;
// void startTimer() {
//   timer = millis();
// }

// void endTimer() {
//   timer = 0;
//   if (old_brightness>0) {
//     brightness = old_brightness;
//     old_brightness = 0;
//   }
// }

int set_rgb(String rgb) {
  sscanf(rgb, "#%02x%02x%02x", &r_value, &g_value, &b_value);
  //Particle.publish("rgb_value",rgb);
  calc_rgbw();
  return 0;
}

int set_w(String w) {
  w_value = w.toInt();
  calc_rgbw();
  return 0;
}

int set_schedule(String p) {
  if (p == "on" || p=="off") {
    schedule = p;
    return 0;
  } else {
    return 1;
  }
}

int set_mode(String p) {
  mode = p;
  Serial.printlnf("mode set: %s", mode.c_str() );
  return 0;
}

int set_random_mode(String p) {
  random_mode = p;
  Serial.printlnf("random_mode is %s", random_mode.c_str() );
  return 0;
}

// int set_party(String p) {
//   old_brightness=brightness;
//   brightness=255;

//   startTimer();

//   return set_mode("party");
// }

int set_brightness(String b) {
   brightness = b.toInt();
   strip.setBrightness(brightness);
   return 0;
}

int set_party_speed(String s) {
   party_speed = s.toInt();
   return 0;
}

int set_colorcycle_speed(String s) {
   colorcycle_speed = s.toInt();
   return 0;
}

int set_random_mode_wait_seconds(String s) {
  random_mode_wait_seconds = s.toInt();
  super_mode_last_change_millis=-1;
  Serial.printlnf("random_mode_wait_seconds is %d seconds", random_mode_wait_seconds);

  return 0;
}

int set_random_mode_frequency(String s) {
  random_mode_frequency = s.toInt();
  Serial.printlnf("random_mode_frequency is 1 in %d", random_mode_frequency);

  return 0;
}

void off(unsigned long wait ) {
  strip.setBrightness(0);
  strip.show();
  delay(wait);
}

// SETUP
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  calc_rgbw();

  srand (millis());

  Serial.begin(9600);

  bool success_c  = Particle.function("rgb_value", set_rgb);
  bool success_w  = Particle.function("w_value", set_w);
  bool success_sw = Particle.function("schedule", set_schedule);
  bool success_rm = Particle.function("random_mode", set_random_mode);

  bool success_m  = Particle.function("mode", set_mode);
  // bool success_pm  = Particle.function("party", set_party);

  bool success_b  = Particle.function("brightness", set_brightness);
  bool success_ps = Particle.function("party_speed", set_party_speed);
  bool success_cs = Particle.function("color_speed", set_colorcycle_speed);

  bool success_rs = Particle.function("random_mode_wait_seconds", set_random_mode_wait_seconds);
  bool success_mf = Particle.function("random_mode_frequency", set_random_mode_frequency);

  // Allow reading from the photon the existing settings
  Particle.variable("schedule", schedule);
  Particle.variable("random_mode", random_mode);
  Particle.variable("mode", mode);
  Particle.variable("brightness", brightness);
  Particle.variable("party_speed", party_speed);
  Particle.variable("color_speed", colorcycle_speed);
  Particle.variable("random_mode_wait_seconds", random_mode_wait_seconds);
  Particle.variable("random_mode_frequency", random_mode_frequency);

  Particle.variable("w_value", w_value);
  Particle.variable("rgbw_value", rgbw_value); // be sure to call calc_rgbw first

  // set the timezone, not it is not DST sensitive.
  Time.zone(11);

}

// MAIN LOOP
int show;
void loop() {
  // Serial.printlnf("millis %lu, mode %s", millis(), mode.c_str());

  show=0;

  // Lights off as scheduled between 4am and 5pm
  if (schedule == "on") {
    // if on schedule and between 5pm and 4am, then turn on, else turn off
    // imagine hour is 0....
    // if( Time.hour() > time_off && Time.hour() < time_on  ) {
      int t =13;
    if( t > time_off && t < time_on  ) {
      Serial.printlnf("schedule is 2 %s. Time is %d", schedule.c_str(), t );
      
      // sleep a second then abort the whole loop
      off(1000);
      return; 
    }

  }

  // if off then sleep, return...
  if (mode == "off") {
      Serial.printlnf("mode is off. sleep 1s %ld.", millis() );

      off(1000);
      return; 
  }

  if (random_mode == "on") {
    // 1000=1s, 3000=3s, 10000=10s, 60000=60s/1m, 600000=600s/10m
    long current_millis = millis()/(random_mode_wait_seconds * 1000.0);

    if (current_millis > super_mode_last_change_millis ) {

      super_mode_last_change_millis = current_millis;

      // 1 in X chance of going into colorcycle
      // random_mode_frequency, 2 (50%), 4 (25%), 6 (16%), 60 (1.6%)
      if ( ! (rand() % random_mode_frequency) ) {
        set_mode("colorcycle"); //mode = "colorcycle";
      }

      // 10 times less likely of going into party
      else if ( ! (rand() % (random_mode_frequency*10)) ) { 
        set_mode("party");
      }

      else {
        set_mode("natural");
      }
    }
    
  }

  if (mode == "party") {
     rainbowCycle(party_speed);
  }

  if (mode == "colorcycle") {
     rainbow(colorcycle_speed);
    //  float mix = (float)mix_tick/(float)mix_duration;
    //  mixin(strip.Color(0,0,0,150), mix);
    //  mix_tick++;
    //  if (mix_tick > mix_duration ) { mix_tick=0; }
  }

  if (mode == "colorwhite") {
    whiteOverRainbow(3,50,1);
  }

  if (mode == "color" ) {
     colorAll(strip.Color(g_value,r_value,b_value,w_value), 1);
  }
  
  else if (mode == "natural") {

    // this is a nice natural orange yellow glow
    // int r_value = 255;
    // int g_value = 106;
    // int b_value = 0;
    // int w_value = 60;

     colorAll(strip.Color(106,255,0,60), 1); 
  }

  if (show) {
    strip.show();
  }

  // 1000*timeOutSeconds = 3000
  // startTimer = 123456
  // if millis
  // at end of party mode we go back to natural after timeOutSecond

  // unsigned long now = millis();
  // if ( timer>0 && now > timer + timeOutSeconds*1000 ) {
  //   endTimer();
  //   set_mode("natural");
  // }

}

void calc_rgbw() {
  rgbw_value = r_value;
  rgbw_value = (rgbw_value << 8) + g_value;
  rgbw_value = (rgbw_value << 8) + b_value;
  rgbw_value = (rgbw_value << 8) + w_value;
}

// uint32_t makeColor(r,g,b,w) {

//   unit8_t r = (c >>  0) & 0xFF;
//   unit8_t g = (c >>  8) & 0xFF;
//   unit8_t b = (c >> 16) & 0xFF;
//   unit8_t w = (v >> 24) & 0xFF;

//   color & 0x000000FF
//   return uint32_t((a << 24) | (r << 16) | (g << 8) | b);
// }


/* ------------------------------------------------------------------------*/
/* --- NEOPIXEL FUNCTIONS                                              ----*/
/* ------------------------------------------------------------------------*/
// Parameter 1 = number of pixels in strip
//               note: for some stripes like those with the TM1829, you
//                     need to count the number of segments, i.e. the
//                     number of controllers in your stripe, not the number
//                     of individual LEDs!
// Parameter 2 = pin number (most are valid)
//               note: if not specified, D2 is selected for you.
// Parameter 3 = pixel type [ WS2812, WS2812B, WS2812B2, WS2813, WS2811,
//                            TM1803, TM1829, SK6812RGBW, WS2812B_FAST,
//                            WS2812B2_FAST ]
//               note: if not specified, WS2812B is selected for you which
//                     is the same as WS2812 or WS2813 in operation.
//               note: RGB order is automatically applied to WS2811,
//                     WS2812/WS2812B/WS2812B2/WS2813/TM1803 is GRB order.
//               note: For legacy 50us reset pulse timing on WS2812/WS2812B
//                     or WS2812B2, select WS2812B_FAST or WS2812B2_FAST
//                     respectively.  Otherwise 300us timing will be used.
//
// 800 KHz bitstream 800 KHz bitstream (most NeoPixel products
//               WS2812/WS2813 (6-pin part)/WS2812B (4-pin part)/SK6812RGBW (RGB+W) )
//
// 400 KHz bitstream (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//                   (Radio Shack Tri-Color LED Strip - TM1803 driver
//                    NOTE: RS Tri-Color LED's are grouped in sets of 3)

void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength ) {

  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int head = whiteLength - 1;
  int tail = 0;
  int loops = 3;
  int loopNum = 0;
  static unsigned long lastTime = 0;

  while(true) {
    for(int j=0; j<256; j++) {
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        if( (i >= tail && i <= head)
          || (tail > head && i >= tail)
          || (tail > head && i <= head) ) {
          strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
        } else {
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
      }

      if(millis() - lastTime > whiteSpeed) {
        head++;
        tail++;
        if(head == strip.numPixels()) {
          loopNum++;
        }
        lastTime = millis();
      }

      if(loopNum == loops) return;

      head %= strip.numPixels();
      tail %= strip.numPixels();
      strip.setBrightness(brightness);
      strip.show();
      delay(wait);
    }
  }

}

// Move all pixels in the strip to a specific color, then wait (ms)
//uint32_t color = strip.getPixelColor(11);
void colorAll(uint32_t c, uint8_t wait) {
  uint16_t i;

  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.setBrightness(brightness);
  strip.show();
  delay(wait);
}

// // Set all pixels in the strip to a solid color, then wait (ms)
// void colorAll(uint32_t c, uint8_t wait) {
//   uint16_t i;

//   for(i=0; i<strip.numPixels(); i++) {
//     strip.setPixelColor(i, c);
//   }
//   strip.setBrightness(brightness);
//   strip.show();
//   delay(wait);
// }

// Fill the dots one after the other with a color, wait (ms) after each one
void colorWipe(uint32_t c, unsigned long wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.setBrightness(brightness);
    strip.show();
    delay(wait);
  }
}

void mixin(uint32_t c, float mix) {

  for(uint16_t i=0; i<strip.numPixels(); i++) {
    uint32_t ce = strip.getPixelColor(i);
    uint32_t c3 = color_add_rgb(c,ce,mix);

    if (i  == 0) {
      String s1 = printColor(c);
      String s2 = printColor(ce);
      String s3 = printColor(c3);
      Serial.printlnf("mix %f. %s + %s = %s", mix, s1.c_str(),s2.c_str(),s3.c_str());
    }
  
    strip.setPixelColor(i, c3);
    // strip.setBrightness(brightness);
    // strip.show();
    // delay(wait);
  }
}

String printColor(uint32_t c) {
    uint8_t w = (uint8_t)((c >> 24) & 0xFF);
    uint8_t r = (uint8_t)((c >> 16) & 0xFF);
    uint8_t g = (uint8_t)((c >>  8) & 0xFF);
    uint8_t b = (uint8_t)((c      ) & 0xFF);
    return String::format("{%d %d %d} {w%d}", r,g,b,w );
}

// unsigned long old_millis = 0;
// uint16_t j = 0;
// void rainbow_orig(int wait) {

//   // manage the delay
//   if (millis() - old_millis >= (unsigned long)wait ) {

//     // if we've waited long enough, change our lights.
//     for(uint16_t i=0; i<strip.numPixels(); i++) {
//       strip.setPixelColor(i, Wheel((j) & 255));
//     }
//     strip.setBrightness(brightness);
//     strip.show();

//     // reset the timer to 300ms
//     // then increment the color after each sleep delay
//     old_millis = millis();
//     if (j++==256) { j = 0; }
//   }
// }

// void rainbow(uint8_t wait) {
//   uint16_t i, j;

//   for(j=0; j<256; j++) {
//     for(i=0; i<strip.numPixels(); i++) {
//       strip.setPixelColor(i, Wheel((i+j) & 255));
//     }
//     strip.setBrightness(brightness);
//     strip.show();
//     delay(wait);
//   }
// }


// make a transition through portions of the rainbow 
// there are 10 parts of the rainbow
int rainbow_portions = 7;
float rainbow_range = 256/rainbow_portions;
unsigned long old_millis = 0;
uint16_t j = 0;
void rainbow(unsigned long wait ) {

  // manage the delay
  if (millis() - old_millis >= wait ) {

    // if we've waited long enough, change our lights.
    // We want to go from j to j+rainbow_range
    int numPixels = strip.numPixels();
    float colorIncrement = rainbow_range / numPixels;
    for(uint16_t i=0; i<numPixels; i++) {
      uint8_t wheel = (uint8_t)(j+colorIncrement*i);
      strip.setPixelColor(i, Wheel(wheel & 255));
    }
    
    show=1;
    // strip.show();

    // reset the timer to 300ms
    // then increment the color after each sleep delay
    old_millis = millis();
    if (j++==256) { j = 0; }
  }
}


// Slightly different, this makes the rainbow equally distributed throughout, then wait (ms)
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) { // 1 cycle of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.setBrightness(brightness);
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t WheelRGBW(byte WheelPos) {
  if(WheelPos < 64) {
   return strip.Color(WheelPos * 4, 255 - WheelPos * 4, 0,0);
  } else if(WheelPos < 128) {
   WheelPos -= 64;
   return strip.Color(255 - WheelPos * 4, 0, WheelPos * 4,0);
  } else if(WheelPos < 192) {
   WheelPos -= 128;
   return strip.Color(0, WheelPos * 4, 255 - WheelPos * 4,0);
  } else {
   WheelPos -= 192;
   return strip.Color(0, 255 - WheelPos * 3, 0,WheelPos * 4);
  }
}
