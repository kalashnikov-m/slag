
#include "huge.h"

#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

static void dump(const std::string& prefix, unsigned long value)
{
    std::cout << prefix << value << "\n";
}

static void dump(const std::string& prefix, const byte* b, const byte* c)
{
    std::cout << prefix;

    while (b != c)
    {
        printf("%02x:", *b);

        ++b;
    }

    printf("\n");
}

#define BUF_SIZE 8

/*static int heighBitIdx(byte x)
{
    int result = -1;

    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;

    x = x - (x >> 1);

    for (; x != 0; x >>= 1)
    {
        ++result;
    }

    return result;
}*/

byte* uadd(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    byte           carry = 0;
    unsigned short tmp   = 0;

    --last1, --last2;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2)
    {
        tmp         = (*last1) + (*last2) + (carry);
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

    return result;
}

byte* usub(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    unsigned short carry = 0;

    --last1, --last2;

    for (; (last1 >= first1) && (last2 >= first2); --last1, --last2)
    {
        if (*last1 < *last2 + carry)
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
        *(--result) = *last1 - carry;
        carry       = 0;
    }

    return result;
}

/*bool ssub(byte* result1, byte* result2, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    short cmp = ucmp(first1, last1, first2, last2);
    
    // a == b
    if(cmp == 0)
    {
	std::fill(result1, result2, 0x00);
	return false;
    }

    // a < b
    if(cmp == -1)
    {	
	usub(result2, first2, last2, first1, last1);
	return true;
    }

    // a > b
    usub(result2, first1, last1, first2, last2);
    
    return false;
}*/

byte* umul(byte* first_result, byte* last_result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    byte  a[BUF_SIZE] = { 0x00 };
    byte  b[BUF_SIZE] = { 0x00 };
    byte* iter        = nullptr;

    std::copy_backward(first1, last1, std::end(a));
    std::copy_backward(first2, last2, std::end(b));

    while (!is_zero(std::begin(b), std::end(b)))
    {
        if (get_low_bit(std::begin(b), std::end(b)) == 0x01)
        {
            iter = uadd(last_result, first_result, last_result, std::begin(a), std::end(a));

            // dump(first_result, last_result);
        }

        shift_left(std::begin(a), std::end(a));
        shift_right(std::begin(b), std::end(b));
    }

    return iter;
}


static void ushort2bytes(byte* result, unsigned short x)
{
    *(--result) = x & 0xff;
    *(--result) = (x >> 8) & 0xff;
}

byte* udiv(byte* div_first, byte* div_last, byte* rem_first, byte* rem_last, const byte* first1, const byte* last1, const byte* first2,
           const byte* last2)
{
    byte* ret     = nullptr;

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

    ret = div_last;

    while (shift > 0)
    {
        unsigned char  Down = 0x00;
        unsigned short Up   = 0x0100;

        // dump("r: ", r_first, r_last);
        // dump("d: ", d_first, d_last);
        auto cmp = ucmp(r_first, r_last, d_first, d_last);

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

            ushort2bytes(std::end(middle), Middle);

            umul(std::begin(mul), std::end(mul), std::begin(middle), std::end(middle), d_first, d_last);

            short mulr_cmp = ucmp(std::begin(mul), std::end(mul), r_first, r_last);

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
        }

        byte tmp[BUF_SIZE] = { 0x00 };
        byte down[]        = { 0x00, 0x00 };

        ushort2bytes(std::end(down), Down);

        // Down * d
        // dump("[r_first, r_last]: ", r_first, r_last);
        umul(std::begin(tmp), std::end(tmp), d_first, d_last, std::begin(down), std::end(down));

        // r - (Down * d);
        r_first = usub(r_last, r_first, r_last, std::begin(tmp), std::end(tmp));

        *(div_last++) = Down;

        --shift;
    }

    std::copy_backward(r_first, r_last, rem_last);

    return ret;
}

byte* uincrement(byte* first, byte* last) 
{
    unsigned short tmp = 0x00;
    byte carry = 0x00;

    --last;

    tmp = (*last) + 0x01 + carry;
    *(last) = (byte)tmp;
    carry = tmp >> 8;

    --last;

    for(;first <= last && carry; --last)
    {
	tmp = (*last) + carry;
	*(last) = (byte)tmp;
	carry = tmp >> 8;
    }

    return last;
}

byte* udecrement(byte* first, byte* last)
{
    unsigned short carry = 0;

    --last;

    if (*last < 0x01)
    {
        *(last) = (*last) - (0x01) - carry + 256;
        carry       = 1;
    }
    else
    {
        *(last) = (*last) - (0x01) - carry;
        carry       = 0;
    }

    --last;
    for (; (first <= last); --last )
    {
        if (*last < carry)
        {
            *(last) = (*last) - carry + 256;
            carry       = 1;
        }
        else
        {
            *(last) = (*last) - carry;
            carry       = 0;
        }
    }
}

bool is_odd(byte* first, byte* last)
{
    return *(--last) & 0x01 == 0x01;
}

bool is_even(byte* first, byte* last) 
{
    return *(--last) & 0x01 == 0x00;
}

void reverse(byte* first, byte* last) 
{
    std::reverse(first, last);
}

void rotate_left(byte* first, byte* last)
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

void rotate_right(byte* first, byte* last)
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

void shift_right(byte* first, byte* last)
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

void shift_left(byte* first, byte* last)
{
    int carry = 0;

    --last;

    for (; first <= last; --last)
    {
        int z = (*last >> 7) & 0x01;

        *last <<= 1;
        *last |= (byte) carry;
        carry = z;
    }
}

void rotate_left_n(byte* first, byte* last, int n)
{
    while (n--)
    {
        rotate_left(first, last);
    }
}

void rotate_right_n(byte* first, byte* last, int n)
{
    --last;

    while (n--)
    {
        rotate_right(first, last);
    }
}

void shift_left_n(byte* first, byte* last, int n)
{
    while (n--)
    {
        shift_left(first, last);
    }
}

void shift_right_n(byte* first, byte* last, int n)
{
    while (n--)
    {
        shift_right(first, last);
    }
}

void op_or(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    --last1; --last2;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2/*, --result*/)
    {
        *(--result) = *last1 | *last2;
    }

    for (; first1 <= last1; --last1/*, --result*/)
    {
        *(--result) = *first1;
    }

    for (; first2 <= last2; --last2/*, --result*/)
    {
        *(--result) = *first2;
    }
}

void op_xor(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    --last1, --last2;//, --result;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2/*, --result*/)
    {
        *(--result) = (*last1) ^ (*last2);
    }

    for (; first1 <= last1; --last1/*, --result*/)
    {
        *(--result) = 0x00;//(*first1);
    }

    for (; first2 <= last2; --last2/*, --result*/)
    {
        *(--result) = 0x00; //(*first2);
    }
}

void op_and(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    --last1, --last2;//, --result;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2/*, --result*/)
    {
        *(--result) = (*last1) & (*last2);
    }

    for (; first1 <= last1; --last1/*, --result*/)
    {
        *(--result) = 0x00;    // *first1;
    }

    for (; first2 <= last2; --last2/*, --result*/)
    {
        *(--result) = 0x00;    // *first2;
    }
}

void op_not(byte* first, byte* last)
{
    for (; first != last; ++first)
    {
        *first = ~(*first);
    }
}

short ucmp(const byte* first1, const byte* last1, const byte* first2, const byte* last2)
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

bool is_zero(const byte* first, const byte* last)
{
    for (; (first != last) && (*first == 0x00); ++first);

    return first == last;
}

int get_low_bit(const byte* first, const byte* last)
{
    return ((*std::prev(last)) & 0x01);
}

int get_high_bit(const byte* first, const byte* last)
{
    return (*first & 0x80);
}

/*byte* clone(byte* result, const byte* first, const byte* last)
{
    result = std::copy(first, last, result);

    return result;
}*/



