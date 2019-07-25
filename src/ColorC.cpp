// #include <iostream>
// using namespace std;
#include <math.h>       /* round, floor, ceil, trunc */
#include "ColorC.h"

// FROM: https://www.rapidtables.com/convert/color/cmyk-to-rgb.html

// #include <iostream>
// using namespace std;

// #include <algorithm>
// #include <initializer_list>

uint32_t packColor(Rgb col) {
    return packColor(col.r, col.g, col.b, col.w);
}

uint32_t packColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    uint32_t cc=0;
    cc |= (w & 255) << 24;
    cc |= (r & 255) << 16;
    cc |= (g & 255) << 8;
    cc |= (b & 255);
    return cc;
}

Rgb unpackColor(uint32_t c)
{
    Rgb cc;
    cc.w = (uint8_t)((c >> 24) & 0xFF);
    cc.r = (uint8_t)((c >> 16) & 0xFF);
    cc.g = (uint8_t)((c >>  8) & 0xFF);
    cc.b = (uint8_t)((c      ) & 0xFF);
    return cc;
}

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
    cmyk.w = rgb.w;

    return cmyk;
}

Rgb cmyk_to_rgb(Cmyk cmyk) {
    float c= clamp(cmyk.c,0,100)/100.0;
    float m= clamp(cmyk.m,0,100)/100.0;
    float y= clamp(cmyk.y,0,100)/100.0;
    float k= clamp(cmyk.k,0,100)/100.0;

    // cout << c << "," << m << "," << y << "," << k << endl;

    Rgb rgb;
    rgb.r = round((1.0-c)*(1.0-k)*255.0);
    rgb.g = round((1.0-m)*(1.0-k)*255.0);
    rgb.b = round((1.0-y)*(1.0-k)*255.0);
    rgb.w = cmyk.w;
    return rgb;
}

// https://codebeautify.org/rgb-to-cmyk-converter
// function RgbToCmyk(r, g, b) {
//     if (0 == r && 0 == g && b == a)
//         return [0, 0, 0, 1];
//     var r2 = 1 - r / 255
//       , g2 = 1 - g / 255
//       , b2 = 1 - b / 255
//       , k = Math.min(r2, Math.min(g2, b2)),
//       , c = (r2 - k) / (1 - k)
//       , m = (g2 - k) / (1 - k)
//       , y = (b2 - k) / (1 - k);
//     return c.toFixed(4) + "," + m.toFixed(4) + "," + y.toFixed(4) + "," + k.toFixed(4)
// }
// function cmyk2rgb(e, h, a, m) {
//     var r = Number(e)
//       , b = Number(h)
//       , c = Number(a)
//       , n = Number(m);
//     return 0 < r ? r /= 100 : 0 < b ? b /= 100 : 0 < c ? c /= 100 : 0 < n && (n /= 100),
//     rgb_r = 1 - Math.min(1, r * (1 - n) + n),
//     rgb_g = 1 - Math.min(1, b * (1 - n) + n),
//     rgb_b = 1 - Math.min(1, c * (1 - n) + n),
//     rgb_r = Math.round(255 * rgb_r),
//     rgb_g = Math.round(255 * rgb_g),
//     rgb_b = Math.round(255 * rgb_b),
//     rgb_r + "," + rgb_g + "," + rgb_b
// }


uint32_t color_add_rgb(uint32_t c1, uint32_t c2, float mix) {
    Rgb c3 = color_add_rgb( unpackColor(c1), 
                            unpackColor(c2), mix);
    return packColor(c3);
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
    c3.w = lerp(c1.w,c2.w,mix);
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

// linear interpolation between point a and point b by c (0.0-1.0).
int lerp( int a, int b, float c ) {
   return round((b - a) * c + a);
}

// char *rgbtostr(Rgb c) {
//     char str[80];
//     sprintf(str, "{%d %d %d} {w%d}",c.r,c.g,c.b,c.w);
//     return str;
// }

// string rgbtostr(Rgb c) {
//     // string greet = String.Format("Hello {0}!", place);
//     ostringstream os;
//     os << "{" << c.r << " " << c.g << " " << c.b << "} {w" << c.w << "}";
//     return os.str();
//     // return s;
// }

// string rgbtostrUint32(uint32_t c) {
//     return rgbtostr(unpackColor(c));
// }

