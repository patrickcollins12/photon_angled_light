#ifndef COLOR_C
#define COLOR_C
 
 struct Cmyk {
   int c;
   int m;
   int y;
   int k;
};

struct Rgb {
   int r;
   int g;
   int b;
};

Cmyk rgb_to_cmyk(Rgb);
Rgb cmyk_to_rgb(Cmyk);

int clamp (int n, int lower, int upper);
float max3( float a, float b, float c );
Rgb color_add(Rgb r1, Rgb r2, float mix);
int lerp( int a, int b, float c );
Rgb color_add_rgb(Rgb r1, Rgb r2, float mix);
Cmyk color_add_cmyk(Cmyk c1, Cmyk c2, float mix);

#endif