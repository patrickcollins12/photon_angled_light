#ifndef COLOR_C
#define COLOR_C

#include <stdint.h>
// #include <stdio.h>
// #include <iostream>
// #include <string>
// #include <sstream>

// using namespace std;

// using namespace std;


 struct Cmyk {
   int c;
   int m;
   int y;
   int k;

   int w;
};

struct Rgb {
   int r;
   int g;
   int b;

   int w;
};

Cmyk rgb_to_cmyk(Rgb);
Rgb cmyk_to_rgb(Cmyk);

int clamp (int n, int lower, int upper);
float max3( float a, float b, float c );
Rgb color_add(Rgb r1, Rgb r2, float mix);
int lerp( int a, int b, float c );


Rgb color_add_rgb(Rgb r1, Rgb r2, float mix);
uint32_t color_add_rgb(uint32_t c1int, uint32_t c2int, float mix);

Cmyk color_add_cmyk(Cmyk c1, Cmyk c2, float mix);

uint32_t packColor(Rgb col);
uint32_t packColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
Rgb unpackColor(uint32_t c);
// char *rgbtostr(Rgb c);
// string rgbtostr(Rgb c);
// string rgbtostrUint32(uint32_t c);

#endif