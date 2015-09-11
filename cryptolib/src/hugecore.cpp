
#include "hugecore.h"

#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <iterator>

using namespace std;

#define BUF_SIZE 8

void HUGE_Add(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    byte           carry = 0;
    unsigned short tmp   = 0;

    --last1;
    --last2;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2)
    {
        tmp         = (*(last1)) + (*(last2)) + (carry);
        *(--result) = (byte) tmp;
        carry       = tmp >> 8;
    }

    for (; first1 <= last1; --last1)
    {
        tmp         = (*last1) + (carry);
        *(--result) = (byte) tmp;
        carry       = tmp >> 8;
    }

    for (; first2 <= last2; --last2)
    {
        tmp         = (*last2) + (carry);
        *(--result) = (byte) tmp;
        carry       = tmp >> 8;
    }
}

void HUGE_Subtract(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    unsigned short carry = 0;

    --last1;
    --last2;

    for (; (last1 >= first1) && (last2 >= first2); --last1, --last2)
    {
        if (*(last1) < *(last2) +carry)
        {
            *(--result) = (*last1) - (*last2) - carry + 256;
            carry       = 1;
        }
        else
        {
            *(--result) = (*last1) - (*last2) - carry;
            carry       = 0;
        }
    }

    for (; first1 <= last1; --last1)
    {
        *(--result) = *(last1) -carry;
        carry       = 0;
    }
}

void HUGE_Multiply(byte* first_result, byte* last_result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    byte a[BUF_SIZE] = { 0x00 };
    byte b[BUF_SIZE] = { 0x00 };

    std::copy_backward(first1, last1, std::end(a));
    std::copy_backward(first2, last2, std::end(b));

    while (!HUGE_IsZero(std::begin(b), std::end(b)))
    {
        if (HUGE_GetLowestSetBit(std::begin(b), std::end(b)) == 0x01)
        {
            HUGE_Add(last_result, first_result, last_result, std::begin(a), std::end(a));
        }

        HUGE_ShiftLeft(std::begin(a), std::end(a));
        HUGE_ShiftRight(std::begin(b), std::end(b));
    }
}

static void ushort2bytes(byte* result, unsigned short x)
{
    *(--result) = x & 0xff;
    *(--result) = (x >> 8) & 0xff;
}

void HUGE_DivRem(byte* div_first, byte* div_last, byte* rem_first, byte* rem_last, const byte* first1, const byte* last1, const byte* first2,
                 const byte* last2)
{
    byte* r_first = nullptr;
    byte* r_last  = nullptr;
    byte* d_first = nullptr;
    byte* d_last  = nullptr;

    while ((first1 != last1) && (*first1 == 0x00))
    {
        ++first1;
    }

    while ((first2 != last2) && (*first2 == 0x00))
    {
        ++first2;
    }

    // dump(first1, last1);
    // dump(first2, last2);
    auto d1    = std::distance(first1, last1);
    auto d2    = std::distance(first2, last2);
    auto shift = d1 - d2;
    r_first    = (byte*) first1;
    r_last     = (byte*) r_first + d2;
    d_first    = (byte*) first2;
    d_last     = (byte*) last2;

    std::advance(div_last, -shift);

    // ret = div_last;
    while (shift > 0)
    {
        unsigned char  Down = 0x00;
        unsigned short Up   = 0x0100;

        // dump("r: ", r_first, r_last);
        // dump("d: ", d_first, d_last);
        auto cmp = HUGE_Compare(r_first, r_last, d_first, d_last);

        if (cmp == -1)
        {
            ++r_last;
        }

        // dump("[r_first..r_last]: ", r_first, r_last);
        // dump("[d_first..d_last]: ", d_first, d_last);
        for (; Down < Up - 1; )
        {
            // dump("down: ", Down);
            // dump("up:   ", Up);
            // 1. c <-- (down + up) / 2;
            auto Middle = (Down + Up) >> 1;

            // 2. mul <-- d * c;
            byte mul[BUF_SIZE] = { 0x00 };
            byte middle[]      = { 0x00, 0x00 };

            // printf("------1------\n");
            ushort2bytes(std::end(middle), Middle);

            // printf("------1.1------\n");
            HUGE_Multiply(std::begin(mul), std::end(mul), std::begin(middle), std::end(middle), d_first, d_last);

            // printf("------2------\n");
            short mulr_cmp = HUGE_Compare(std::begin(mul), std::end(mul), r_first, r_last);

            // printf("------3------\n");
            if (mulr_cmp == -1)
            {    // if(c < a): down <-- c
                Down = Middle;
            }
            else if (mulr_cmp == 1)
            {    // if(c > a) Up <-- c
                Up = Middle;
            }
            else if (mulr_cmp == 0)
            {    // if(mul == a) Up <-- C; Down <-- Up;
                Up   = Middle;
                Down = Up;
            }

            // printf("------4------\n");
        }

        byte tmp[BUF_SIZE] = { 0x00 };
        byte down[]        = { 0x00, 0x00 };

        ushort2bytes(std::end(down), Down);

        // Down * d
        // dump("[r_first..r_last]: ", r_first, r_last);
        HUGE_Multiply(std::begin(tmp), std::end(tmp), d_first, d_last, std::begin(down), std::end(down));

        // r - (Down * d);

        HUGE_Subtract(r_last, r_first, r_last, std::begin(tmp), std::end(tmp));

        *(div_last++) = Down;

        --shift;
    }

    std::copy_backward(r_first, r_last, rem_last);
}

void HUGE_Increment(byte* first, byte* last)
{
    unsigned short tmp   = 0x00;
    byte           carry = 0x00;

    tmp     = (*--last) + 0x01 + carry;
    *(last) = (byte) tmp;
    carry   = tmp >> 8;

    for (; (first <= last) && carry; )
    {
        tmp     = (*--last) + carry;
        *(last) = (byte) tmp;
        carry   = tmp >> 8;
    }
}

void HUGE_Decrement(byte* first, byte* last)
{
    unsigned short carry = 0;

    if (*--last < 0x01)
    {
        *(last) = (*last) - (0x01) - carry + 256;
        carry   = 1;
    }
    else
    {
        *(last) = (*last) - (0x01) - carry;
        carry   = 0;
    }

    for (; (first <= last); )
    {
        if (*--last < carry)
        {
            *(last) = (*last) - carry + 256;
            carry   = 1;
        }
        else
        {
            *(last) = (*last) - carry;
            carry   = 0;
        }
    }
}

bool HUGE_IsOdd(byte* first, byte* last)
{
    return (*(--last) & 0x01) == 0x01;
}

bool HUGE_IsEven(byte* first, byte* last)
{
    return (*(--last) & 0x01) == 0x00;
}

void HUGE_Reverse(byte* first, byte* last)
{
    std::reverse(first, last);
}

void HUGE_RotateLeft(byte* first, byte* last)
{
    int carry = (*first >> 7) & 0x01;

    for (; first != last; --last)
    {
        int z = (*last >> 7) & 0x01;

        *last <<= 1;
        *last |= (byte) carry;
        carry = z;
    }
}

void HUGE_RotateRight(byte* first, byte* last)
{
    int carry = *(last - 1) & 0x01;    // *(std::prev(last)) & 0x01;

    for (; first != last; ++first)
    {
        int z = *first & 0x01;

        *first >>= 1;
        *first |= (byte) (carry << 7);
        carry  = z;
    }
}

void HUGE_ShiftRight(byte* first, byte* last)
{
    int carry = 0;

    for (; first != last; ++first)
    {
        int z = (*first) & 0x01;

        *first >>= 1;
        *first |= (byte) (carry << 7);
        carry  = z;
    }
}

void HUGE_ShiftLeft(byte* first, byte* last)
{
    int carry = 0;

    --last;

    for (; first <= last; --last)
    {
        int z = (*(last) >> 7) & 0x01;

        *last <<= 1;
        *last |= (byte) carry;
        carry = z;
    }
}

void HUGE_RotateLeftN(byte* first, byte* last, int n)
{
    while (n--)
    {
        HUGE_RotateLeft(first, last);
    }
}

void HUGE_RotateRightN(byte* first, byte* last, int n)
{
    --last;

    while (n--)
    {
        HUGE_RotateRight(first, last);
    }
}

void HUGE_ShiftLeftN(byte* first, byte* last, int n)
{
    while (n--)
    {
        HUGE_ShiftLeft(first, last);
    }
}

void HUGE_ShiftRightN(byte* first, byte* last, int n)
{
    while (n--)
    {
        HUGE_ShiftRight(first, last);
    }
}

void HUGE_Or(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    for (; (first1 <= last1) && (first2 <= last2); )
    {
        *(--result) = *(--last1) | *(--last2);
    }

    for (; first1 <= last1; --last1)
    {
        *(--result) = *first1;
    }

    for (; first2 <= last2; --last2)
    {
        *(--result) = *first2;
    }
}

void HUGE_Xor(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    for (; (first1 <= last1) && (first2 <= last2); )
    {
        *(--result) = (*--last1) ^ (*--last2);
    }

    for (; first1 <= last1; --last1)
    {
        *(--result) = 0x00;
    }

    for (; first2 <= last2; --last2)
    {
        *(--result) = 0x00;
    }
}

void HUGE_And(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    for (; (first1 <= last1) && (first2 <= last2); )
    {
        *(--result) = (*--last1) & (*--last2);
    }

    for (; first1 <= last1; --last1)
    {
        *(--result) = 0x00;
    }

    for (; first2 <= last2; --last2)
    {
        *(--result) = 0x00;
    }
}

void HUGE_Not(byte* first, byte* last)
{
    for (; first != last; ++first)
    {
        *first = ~(*first);
    }
}

short HUGE_Compare(const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    for (; (first1 != last1) && (*first1 == 0); ++first1);
    for (; (first2 != last2) && (*first2 == 0); ++first2);
    for (; (first1 != last1) && (first2 != last2) && (*first1 == *first2); ++first1, ++first2);

    auto dA = std::distance(first1, last1);
    auto dB = std::distance(first2, last2);

    if (dA == dB)
    {
        if ((first1 < last1) && (first2 < last2))
        {
            return (short) ((*first1 < *first2) ? -1 : 1);
        }
    }

    if (dA < dB)
    {
        return -1;
    }

    if (dA > dB)
    {
        return 1;
    }

    return 0;
}

bool HUGE_IsZero(const byte* first, const byte* last)
{
    for (; (first != last) && (*first == 0x00); ++first);

    return first == last;
}

int HUGE_GetLowestSetBit(const byte* first, const byte* last)
{
    return ((*std::prev(last)) & 0x01);
}

int HUGE_GetHighestSetBit(const byte* first, const byte* last)
{
    return (*first & 0x80);
}
