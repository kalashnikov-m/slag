/* 
 * File:   main.cpp
 * Author: max
 *
 * Created on 7 Май 2015 г., 8:38
 */

#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <iostream>

#include "huge.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    byte a[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10};
    byte b[] = {0x00, 0x00, 0x00, 0x10};
    byte c[8] = {0x00};
    byte d[8] = {0x00};

    /*shift_left(begin(a), end(a));
    for_each(begin(a), end(a), [](byte bt)
    {
        printf("%02x.", bt);
    });
    printf("\n");*/


    byte* result = udiv(std::begin(c), std::end(c),
            std::begin(d), std::end(d),
            begin(a), end(a),
            begin(b), end(b));

    for_each(begin(c), end(c), [](byte b)
    {
        //printf("%02x.", b);
    });
    printf("\n");

    /*byte* result = umul(begin(c), end(c),
            begin(a), end(a),
            begin(b), end(b));

    for_each(begin(c), result, end(c), [](byte b)
    {
        //printf("%02x.", b);
    });
    printf("\n");*/

    //cout << get_high_bit(begin(a), end(a));

    //cout << ucmp(begin(a), end(a), begin(b), end(b));

    /*auto result = usub(end(c), begin(a), end(a), begin(b), end(b));
    for_each(result, end(c), [](byte bt)
    {
        printf("%02x.", bt);
    });

    printf("\n");*/

    /*rotate_right_n(begin(a), end(a), 1);
    rotate_right(begin(a), end(a));
    for_each(begin(a), end(a), [](byte bt)
    {
        printf("%02x.", bt);
    });
    printf("\n");*/

    /*shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));
    shift_right(begin(a), end(a));*/

    /*shift_right_n(begin(a), end(a), 33);
    for_each(begin(a), end(a), [](byte bt)
    {
        printf("%02x.", bt);
    });
    printf("\n");*/

    /*byte* result = usub(c + lc, a, a + la, b, b + lb);
    for_each(begin(c), end(c), [](byte b)
    {
        printf("%02x.", b);
    });*/

    //printf("\n");

    /*byte* result = uadd(end(c), 
            begin(a), end(a), 
            begin(b), end(b));
    
    for_each(result, end(c), [](byte b)
    {
        printf("%02x:", b);
    });*/

    //printf("\n");

    return 0;
}
