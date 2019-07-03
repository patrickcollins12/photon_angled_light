#include "ColorC.h"
// #include <iostream>
// using namespace std;
#include <math.h>       /* round, floor, ceil, trunc */

// FROM: https://www.rapidtables.com/convert/color/cmyk-to-rgb.html

// #include <iostream>
// using namespace std;

// #include <algorithm>
// #include <initializer_list>

Cmyk rgb_to_cmyk(Rgb rgb) {
    Cmyk cmyk;

    float r = clamp(rgb.r, 0, 255) / 255.0;
    float g = clamp(rgb.g, 0, 255) / 255.0;
    float b = clamp(rgb.b, 0, 255) / 255.0;

    // tmp floats 0..1
    float c,m,y,k;

    k = 1.0 - max3(r,g,b);
    
    if( k==1 ) {
        c=m=y=0;
    } else {
        c = (1.0-r-k)/(1.0-k);
        m = (1.0-g-k)/(1.0-k);
        y = (1.0-b-k)/(1.0-k);
    }

    // convert up to 0-100 ints
    cmyk.c = round(c * 100);
    cmyk.m = round(m * 100);
    cmyk.y = round(y * 100);
    cmyk.k = round(k * 100);

    return cmyk;
}

Rgb cmyk_to_rgb(Cmyk cmyk) {
    float c= clamp(cmyk.c,0,100)/100.0;
    float m= clamp(cmyk.m,0,100)/100.0;
    float y= clamp(cmyk.y,0,100)/100.0;
    float k= clamp(cmyk.k,0,100)/100.0;

    // cout << c << "," << m << "," << y << "," << k << endl;

    Rgb rgb;
    rgb.r = round((1.0-c)*(1.0-k)*255);
    rgb.g = round((1.0-m)*(1.0-k)*255);
    rgb.b = round((1.0-y)*(1.0-k)*255);
    return rgb;
}

Rgb color_add_rgb(Rgb r1, Rgb r2, float mix) {
    Cmyk c1 = rgb_to_cmyk(r1);
    Cmyk c2 = rgb_to_cmyk(r2);
    Cmyk c3 = color_add_cmyk(c1,c2,mix);
    return cmyk_to_rgb(c3);
}

Cmyk color_add_cmyk(Cmyk c1, Cmyk c2, float mix) {
    Cmyk c3;
    c3.c = lerp(c1.c,c2.c,mix);
    c3.m = lerp(c1.m,c2.m,mix);
    c3.y = lerp(c1.y,c2.y,mix);
    c3.k = lerp(c1.k,c2.k,mix);
    return c3;
}

// Math Helpers
int clamp (int n, int lower, int upper) {
    return n <= lower ? lower : n >= upper ? upper : n;
}

float max3( float a, float b, float c ) {
   float max = ( a < b ) ? b : a;
   return ( ( max < c ) ? c : max );
}

int lerp( int a, int b, float c ) {
   return round((b - a) * c + a);
}

