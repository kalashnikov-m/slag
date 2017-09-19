
#include "HugeCore.h"

#include <cstdint>
#include <iterator>
#include <vector>

void Cry_add(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    byte carry = 0;
    unsigned short tmp;

    --last1;
    --last2;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2)
    {
        tmp         = (*(last1)) + (*(last2)) + (carry);
        *(--result) = static_cast<byte>(tmp);
        carry       = tmp >> 8;
    }

    for (; first1 <= last1; --last1)
    {
        tmp         = (*last1) + (carry);
        *(--result) = static_cast<byte>(tmp);
        carry       = tmp >> 8;
    }

    for (; first2 <= last2; --last2)
    {
        tmp         = (*last2) + (carry);
        *(--result) = static_cast<byte>(tmp);
        carry       = tmp >> 8;
    }

    if (carry)
        *(--result) = static_cast<byte>(carry);
}

void Cry_subtract(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    unsigned short carry = 0;

    --last1;
    --last2;

    for (; (last1 >= first1) && (last2 >= first2); --last1, --last2)
    {
        if (*(last1) < *(last2) + carry)
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
        *(--result) = *(last1)-carry;
        carry       = 0;
    }
}

void Cry_multiply(byte* last_result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    --last1;
    --last2;

    byte carry = 0x00;

    for (; first2 <= last2; --last2)
    {
        --last_result;

        byte* resultIter = last_result;

        for (const byte* last_1 = last1; first1 <= last_1; --last_1)
        {
            uint16_t temp = (*resultIter) + (*last_1) * (*last2) + carry;

            carry = temp / 256;

            *(resultIter) = temp % 256;

            --resultIter;
        }

        *(resultIter) = carry;
        carry         = 0x00;
    }
}

void Cry_multiply(byte* last_result, const byte* first1, const byte* last1, byte x)
{
    --last1;
    --last_result;

    byte carry = 0x00;

    for (; first1 <= last1; --last1)
    {
        uint16_t temp = (*last1) * x + carry;

        carry          = temp / 256;
        *(last_result) = temp % 256;

        --last_result;
    }

    *last_result = carry;
}

void Cry_divide(byte* div_last, byte* rem_last, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    while ((first1 != last1) && (*first1 == 0x00))
    {
        ++first1;
    }

    while ((first2 != last2) && (*first2 == 0x00))
    {
        ++first2;
    }

    auto rTmp = std::vector<byte>(first1, last1);

    auto d1            = std::distance(first1, last1);
    auto d2            = std::distance(first2, last2);
    auto shift         = d1 - d2 + 1;
    byte* rFirst       = &rTmp[0];
    byte* rLast        = rFirst + d2;
    const byte* dFirst = first2;
    const byte* dLast  = last2;

    std::advance(div_last, -shift);

    auto cmp = Cry_compare(rFirst, rLast, dFirst, dLast);
    if (cmp == -1)
    {
        ++rLast;
        ++div_last;
        --shift;
    }

    size_t nbytes = d2 + 1;
    std::vector<byte> mul(nbytes);

    while (shift > 0)
    {
        byte Down = 0x00;
        uint16_t Up  = 0x00100;

        auto cmp = Cry_compare(rFirst, rLast, dFirst, dLast);
        if (cmp == -1)
        {
            ++rLast;
        }

        for (; Down < Up - 1;)
        {
            // 1. c <-- (down + up) / 2;
            byte Middle = ((Down + Up) / 2);

            // 2. mul <-- d * c;
            std::fill(&mul[0], &mul[0] + nbytes, 0x00);

            Cry_multiply(&mul[0] + nbytes, dFirst, dLast, Middle);

            short mulCmp = Cry_compare(&mul[0], &mul[0] + nbytes, rFirst, rLast);

            if (mulCmp == -1)
            { // if(c < a): down <-- c
                Down = Middle;
            }
            else if (mulCmp == 1)
            { // if(c > a) Up <-- c
                Up = Middle;
            }
            else if (mulCmp == 0)
            { // if(mul == a) Up <-- C; Down <-- Up;
                Up   = Middle;
                Down = static_cast<byte>(Up);
            }
        }

        std::fill(&mul[0], &mul[0] + nbytes, 0x00);

        Cry_multiply(&mul[0] + nbytes, dFirst, dLast, Down);

        Cry_subtract(rLast, rFirst, rLast, &mul[0], &mul[0] + nbytes);

        *(div_last++) = Down;

        --shift;
    }

    std::copy_backward(rFirst, rLast, rem_last);
}

void Cry_increment(byte* first, byte* last)
{
    byte carry = 0x00;

    unsigned short tmp = (*--last) + 0x01 + carry;
    *(last)            = static_cast<byte>(tmp);
    carry              = tmp >> 8;

    for (; (first <= last) && carry;)
    {
        tmp     = (*--last) + carry;
        *(last) = static_cast<byte>(tmp);
        carry   = tmp >> 8;
    }
}

void Cry_decrement(byte* first, byte* last)
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

    for (; (first <= last);)
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

bool Cry_is_odd(const byte* first, const byte* last)
{
    return (*(--last) & 0x01) == 0x01;
}

bool Cry_is_even(const byte* first, const byte* last)
{
    return (*(--last) & 0x01) == 0x00;
}

void Cry_reverse(byte* first, byte* last)
{
    std::reverse(first, last);
}

void Cry_rotl(byte* first, byte* last)
{
    int carry = (*first >> 7) & 0x01;

    for (; first != last; --last)
    {
        int z = (*last >> 7) & 0x01;

        *last <<= 1;
        *last |= static_cast<byte>(carry);
        carry = z;
    }
}

void Cry_rotr(byte* first, byte* last)
{
    int carry = *(last - 1) & 0x01; // *(std::prev(last)) & 0x01;

    for (; first != last; ++first)
    {
        int z = *first & 0x01;

        *first >>= 1;
        *first |= static_cast<byte>(carry << 7);
        carry = z;
    }
}

void Cry_rshift(byte* first, byte* last)
{
    int carry = 0;

    for (; first != last; ++first)
    {
        int z = (*first) & 0x01;

        *first >>= 1;
        *first |= static_cast<byte>(carry << 7);
        carry = z;
    }
}

void Cry_lshift(byte* first, byte* last)
{
    int carry = 0;

    --last;

    for (; first <= last; --last)
    {
        int z = (*(last) >> 7) & 0x01;

        *last <<= 1;
        *last |= static_cast<byte>(carry);
        carry = z;
    }
}

void Cry_rotl(byte* first, byte* last, int n)
{
    while (n--)
    {
        Cry_rotl(first, last);
    }
}

void Cry_rotr(byte* first, byte* last, int n)
{
    --last;

    while (n--)
    {
        Cry_rotr(first, last);
    }
}

void Cry_lshift(byte* first, byte* last, int n)
{
    while (n--)
    {
        Cry_lshift(first, last);
    }
}

void Cry_rshift(byte* first, byte* last, int n)
{
    while (n--)
    {
        Cry_rshift(first, last);
    }
}

void Cry_or(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    --last1;
    --last2;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2)
    {
        *(--result) = *(last1) | *(last2);
    }

    for (; first1 <= last1; --last1)
    {
        *(--result) = *last1;
    }

    for (; first2 <= last2; --last2)
    {
        *(--result) = *last2;
    }
}

void Cry_xor(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    --last1;
    --last2;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2)
    {
        *(--result) = (*last1) ^ (*last2);
    }

    for (; first1 <= last1; --last1)
    {
        *(--result) = *last1;
    }

    for (; first2 <= last2; --last2)
    {
        *(--result) = *last2;
    }
}

void Cry_and(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    --last1;
    --last2;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2)
    {
        *(--result) = *(last1) & *(last2);
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

void Cry_inverse(byte* first, byte* last)
{
    for (; first != last; ++first)
    {
        *first = ~(*first);
    }
}

short Cry_compare(const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    for (; (first1 != last1) && (*first1 == 0);)
    {
        ++first1;
    }

    for (; (first2 != last2) && (*first2 == 0);)
    {
        ++first2;
    }

    for (; (first1 != last1) && (first2 != last2) && (*first1 == *first2); ++first1, ++first2)
        ;

    auto dA = std::distance(first1, last1);
    auto dB = std::distance(first2, last2);

    if (dA == dB)
    {
        if ((first1 < last1) && (first2 < last2))
        {
            return static_cast<short>((*first1 < *first2) ? -1 : 1);
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

bool Cry_equal(const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{

    for (; (first1 != last1) && (*first1 == 0x00); ++first1)
        ;

    for (; (first2 != last2) && (*first2 == 0x00); ++first2)
        ;

    for (; (first1 != last1) && (first2 != last2) && (*first1) == (*first2); ++first1, ++first2)
        ;

    return (first1 == last1) && (first2 == last2);
}

bool Cry_is_zero(const byte* first, const byte* last)
{
    for (; (first != last) && (*first == 0x00);)
    {
        ++first;
    }

    return first == last;
}

int Cry_get_lowest_set_bit(const byte* first, const byte* last)
{
    return ((*std::prev(last)) & 0x01);
}

int Cry_get_highest_set_bit(const byte* first, const byte* last)
{
    return (*first & 0x80);
}

/*bool HUGE_isPowerOfTwo(const byte* first, const byte* last)
{
    return false;
}*/

bool Cry_is_one(const byte* first, const byte* last)
{
    --last;

    if (*last == 0x01)
    {
        --last;

        while ((first <= last) && (*last == 0x00))
        {
            --last;
        }

        return !(first <= last);
    }

    return false;
}
