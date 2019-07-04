// g++ src/ColorC.cpp test/color_test_unit.cpp -o color_test_unit -I src 
//     -std=c++11 && ./color_test_unit

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "ColorC.h"
#include "Catch.hpp"

Rgb c1 = { 100,255,150 };
Cmyk c2 = { 50,50,25,30 };

TEST_CASE( "Lerp", "[basics]" ) {
    REQUIRE( lerp( 10, 20, .5 ) == 15 );
    REQUIRE( lerp( -10, 10, .5 ) == 0 );
    REQUIRE( lerp( 0, 10, .1 ) == 1 );
}

TEST_CASE( "Clamp", "[basics]" ) {
    REQUIRE( clamp( 10, 0, 10 ) == 10 );
    REQUIRE( clamp( 11, 0, 10 ) == 10 );
    REQUIRE( clamp( -1, 0, 10 ) == 0 );
    REQUIRE( clamp(  1, 0, 10 ) == 1 );
}

TEST_CASE( "Rgb struct", "[basics]" ) {
    REQUIRE( c1.r == 100 );
    REQUIRE( c1.g == 255 );
    REQUIRE( c1.b == 150 );
}

TEST_CASE( "Cmyk struct", "[basics]" ) {
    REQUIRE( c2.c == 50 );
    REQUIRE( c2.m == 50 );
    REQUIRE( c2.y == 25 );
    REQUIRE( c2.k == 30 );
}

Rgb c3 = cmyk_to_rgb(c2);
TEST_CASE( "cmyk to rgb", "[basics]" ) {
    REQUIRE( c3.r == 89 );
    REQUIRE( c3.g == 89 );
    REQUIRE( c3.b == 134 );
}

// This should work, it does demonstrate a bug if I can't go back to cmyk.
// Cmyk c2a = rgb_to_cmyk(c3);
// TEST_CASE( "cmyk to rgb back to cmyk", "[basics]" ) {
//     REQUIRE( c2a.c == 50 );
//     REQUIRE( c2a.m == 50 );
//     REQUIRE( c2a.y == 25 );
//     REQUIRE( c2a.k == 30 );
// }



Cmyk c4 = rgb_to_cmyk(c3);
TEST_CASE( "rgb to cmyk", "[basics]" ) {
    REQUIRE( c4.c == 34 );
    REQUIRE( c4.m == 34 );
    REQUIRE( c4.y == 0 );
    REQUIRE( c4.k == 47 );
}

Cmyk c5 = color_add_cmyk(c2,c4,0.5);
TEST_CASE( "cmyk + cmyk", "[basics]" ) {
    REQUIRE( c5.c == 42 );
    REQUIRE( c5.m == 42 );
    REQUIRE( c5.y == 13 );
    REQUIRE( c5.k == 39 );
}

// Rgb c6 = color_add_rgb(c1,c3,0.5);
// Cmyk c7 = rgb_to_cmyk(c6);
// TEST_CASE( "rgb + rgb", "[basics]" ) {
//     REQUIRE( c7.c == 42 );
//     REQUIRE( c7.m == 42 );
//     REQUIRE( c7.y == 13 );
//     REQUIRE( c7.k == 39 );

//     REQUIRE( c6.r == 90 );
//     REQUIRE( c6.g == 90 );
//     REQUIRE( c6.b == 135 );
// }