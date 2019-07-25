// g++ src/ColorC.cpp test/color_test_unit.cpp -o color_test_unit -I src 
//     -std=c++11 && ./color_test_unit

#include <stdint.h>
// #include <string>
#include "ColorC.h"
#include <gtest/gtest.h>

Rgb c1 = { 100,255,150,50 };
Cmyk c2 = { 50,50,25,30,60 };

uint8_t r = 120;
uint8_t g = 150;
uint8_t b = 110;
uint8_t w = 100;
uint32_t cc = packColor(r,g,b,w);


TEST( Basics, Lerp ) {
    ASSERT_EQ( lerp( 10, 20, .5 ),15 );
    ASSERT_EQ( lerp( -10, 10, .5 ), 0 );
    ASSERT_EQ( lerp( 0, 10, .1 ), 1 );
    // EXPECT_EQ( lerp( 0, 10, .1 ), .9 );

}

TEST( Basics, Clamp ) {
    ASSERT_EQ( clamp( 10, 0, 10 ), 10 );
    ASSERT_EQ( clamp( 11, 0, 10 ), 10 );
    ASSERT_EQ( clamp( -1, 0, 10 ), 0 );
    ASSERT_EQ( clamp(  1, 0, 10 ), 1 );
}

TEST( Basics, RgbStruct ) {
    ASSERT_EQ( c1.r, 100 );
    ASSERT_EQ( c1.g, 255 );
    ASSERT_EQ( c1.b, 150 );
    ASSERT_EQ( c1.w,  50 );

}

TEST( Basics, CmykStruct ) {
    ASSERT_EQ( c2.c,  50 );
    ASSERT_EQ( c2.m,  50 );
    ASSERT_EQ( c2.y,  25 );
    ASSERT_EQ( c2.k,  30 );
    ASSERT_EQ( c2.w,  60 );

}

// TEST( Basics, rgbtostr ) {
//     // Rgb c1 = { 100,255,150,50 };
//     EXPECT_EQ( rgbtostr(c1), "{100 255 150} {w50}" );
// }

// TEST( Basics, rgbtostrUint32 ) {
//     // uint8_t r = 120;
//     // uint8_t g = 150;
//     // uint8_t b = 110;
//     // uint8_t w = 100;
//     // uint32_t cc = packColor(r,g,b,w);
//     EXPECT_EQ( rgbtostrUint32(cc), "{120 150 110} {w100}" );
// }

Rgb c3 = cmyk_to_rgb(c2);
TEST( Operations, CmykToRgb2 ) {
    ASSERT_EQ( c3.r,  89 );
    ASSERT_EQ( c3.g,  89 );
    ASSERT_EQ( c3.b,  134 );
    ASSERT_EQ( c3.w,  60 );
}

// This should work, it does demonstrate a bug if I can't go back to cmyk.
// Cmyk c2a = rgb_to_cmyk(c3);
// TEST( "cmyk to rgb back to cmyk", "[basics]" ) {
//     ASSERT_EQ( c2a.c,  50 );
//     ASSERT_EQ( c2a.m,  50 );
//     ASSERT_EQ( c2a.y,  25 );
//     ASSERT_EQ( c2a.k,  30 );
// }

Cmyk c4 = rgb_to_cmyk(c3);
TEST( Operations, RgbToCmyk ) {
    ASSERT_EQ( c4.c,  34 );
    ASSERT_EQ( c4.m,  34 );
    ASSERT_EQ( c4.y,   0 );
    ASSERT_EQ( c4.k,  47 );
    ASSERT_EQ( c4.w,  60 );

}

Cmyk c5 = color_add_cmyk(c2,c4,0.5);
TEST( Operations, CmykPlusCmyk ) {
    ASSERT_EQ( c5.c,  42 );
    ASSERT_EQ( c5.m,  42 );
    ASSERT_EQ( c5.y,  13 );
    ASSERT_EQ( c5.k,  39 );
    ASSERT_EQ( c5.w,  60 );
}

/////////////////////////////////////////
// packColor / unpackColor

TEST( Operations, packColor ) {
    uint32_t cc2=0;
    cc2 |= (w & 255) << 24;
    cc2 |= (r & 255) << 16;
    cc2 |= (g & 255) << 8;
    cc2 |= (b & 255);
    ASSERT_EQ( cc, cc2 );
}

TEST( Operations, packColorRGB ) {
    // uint32_t packColor(Rgb col);
    // Rgb c3 = cmyk_to_rgb(c2); // 89 89 134 ?
    ASSERT_EQ( c3.r,  89 );
    ASSERT_EQ( c3.g,  89 );
    ASSERT_EQ( c3.b,  134 );
    ASSERT_EQ( c3.w,  60 );

    uint32_t xpre = packColor(c3);
    Rgb x = unpackColor(xpre);
    ASSERT_EQ( x.r,  89 );
    ASSERT_EQ( x.g,  89 );
    ASSERT_EQ( x.b,  134 );
    ASSERT_EQ( x.w,  60 );
}

TEST( Operations, unpackColor ) {
    Rgb x = unpackColor(cc);
    ASSERT_EQ( x.r,  120 );
    ASSERT_EQ( x.g,  150 );
    ASSERT_EQ( x.b,  110 );
    ASSERT_EQ( x.w,  100 );
}


// uint32_t color_add_rgb(uint32_t c1, uint32_t c2, float mix) {
//     Rgb c3 = color_add_rgb( unpackColor(c1), 
//                             unpackColor(c2), mix);
//     return packColor(c3);
// }

TEST( Operations, color_add_rgb ) {
    uint32_t c1 = packColor(50,75,95,100);
    uint32_t c2 = packColor(10,20,50,200);
    uint32_t c3 = color_add_rgb(c1,c2,0.5);
    Rgb c4 = unpackColor(c3);
    ASSERT_EQ( c4.r,  26 );
    ASSERT_EQ( c4.g,  42 );
    ASSERT_EQ( c4.b,  71 );
    ASSERT_EQ( c4.w,  150 );

}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// Rgb c6 = color_add_rgb(c1,c3,0.5);
// Cmyk c7 = rgb_to_cmyk(c6);
// TEST( "rgb + rgb", "[basics]" ) {
//     ASSERT_EQ( c7.c,  42 );
//     ASSERT_EQ( c7.m,  42 );
//     ASSERT_EQ( c7.y,  13 );
//     ASSERT_EQ( c7.k,  39 );

//     ASSERT_EQ( c6.r,  90 );
//     ASSERT_EQ( c6.g,  90 );
//     ASSERT_EQ( c6.b,  135 );
// }