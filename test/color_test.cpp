#include "ColorC.h"

#include <iostream>
using namespace std;


int main()
{
    Rgb c1 = { 100,255,150 };
    cout << c1.r << "," << c1.g << "," << c1.b << endl;

    Cmyk c2 = rgb_to_cmyk(c1);
    Rgb c3 = cmyk_to_rgb(c2);

    cout << c3.r << "," << c3.g << "," << c3.b << endl;

    cout << "cmyk1 - " << c2.c << "," << c2.m << "," << c2.y << "," << c2.k << endl;

    Rgb rgb2 = { 200,49,33 };

    Cmyk cmyk2 = rgb_to_cmyk(rgb2);
    cout << "cmyk2 - " << cmyk2.c << "," << cmyk2.m << "," << cmyk2.y << "," << cmyk2.k << endl;

    Cmyk cmyk3;
    cmyk3 = color_add_cmyk(c2,cmyk2,0.5);
    cout << "cmyk3 mix 0.5 - " << cmyk3.c << "," << cmyk3.m << "," << cmyk3.y << "," << cmyk3.k << endl;

    cmyk3 = color_add_cmyk(c2,cmyk2,0.0);
    cout << "cmyk3 mix 0.0 - " << cmyk3.c << "," << cmyk3.m << "," << cmyk3.y << "," << cmyk3.k << endl;

    cmyk3 = color_add_cmyk(c2,cmyk2,1.0);
    cout << "cmyk3 mix 1.0 - " << cmyk3.c << "," << cmyk3.m << "," << cmyk3.y << "," << cmyk3.k << endl;

    cmyk3 = color_add_cmyk(c2,cmyk2,0.1);
    cout << "cmyk3 mix 0.1 - " << cmyk3.c << "," << cmyk3.m << "," << cmyk3.y << "," << cmyk3.k << endl;

    cmyk3 = color_add_cmyk(c2,cmyk2,0.9);
    cout << "cmyk3 mix 0.9 - " << cmyk3.c << "," << cmyk3.m << "," << cmyk3.y << "," << cmyk3.k << endl;

    // TEST_CASE( "CMYK 4 mix 0.9", "[factorial]" ) {
    //     REQUIRE( Factorial(1) == 1 );
    //     REQUIRE( Factorial(2) == 2 );
    //     REQUIRE( Factorial(3) == 6 );
    //     REQUIRE( Factorial(10) == 3628800 );
    // }

    Rgb rgb_add = color_add_rgb(c1,rgb2,0.5);

    cout << rgb_add.r << "," << rgb_add.g << "," << rgb_add.b << endl;

}
