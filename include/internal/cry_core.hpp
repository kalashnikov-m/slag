
#ifndef CRY_CORE_HPP
#define CRY_CORE_HPP

#include <iterator>
#include <vector>

namespace
{
    template <class T>
    struct traits;

    template <>
    struct traits<uint8_t>
    {
        using wide_type             = uint16_t;
        static const wide_type base = UINT8_MAX + 1;
    };

    template <>
    struct traits<uint16_t>
    {
        using wide_type             = uint32_t;
        static const wide_type base = UINT16_MAX + 1;
    };

    template <>
    struct traits<uint32_t>
    {
        using wide_type             = uint64_t;
        static const wide_type base = static_cast<uint64_t>(UINT32_MAX) + 1;
    };
}

template <class byte, class Traits = traits<byte>>
void Cry_add(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    byte carry = 0;
    typedef typename Traits::wide_type wide_t;
    wide_t tmp = 0;

    --last1;
    --last2;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2)
    {
        tmp         = static_cast<wide_t>(*last1) + static_cast<wide_t>(*last2) + static_cast<wide_t>(carry);
        *(--result) = static_cast<byte>(tmp);
        carry       = tmp >> sizeof(carry) * 8;
    }

    for (; first1 <= last1; --last1)
    {
        tmp         = static_cast<wide_t>(*last1) + static_cast<wide_t>(carry);
        *(--result) = static_cast<byte>(tmp);
        carry       = tmp >> sizeof(carry) * 8;
    }

    for (; first2 <= last2; --last2)
    {
        tmp         = static_cast<wide_t>(*last2) + static_cast<wide_t>(carry);
        *(--result) = static_cast<byte>(tmp);
        carry       = tmp >> sizeof(carry) * 8;
    }

    if (carry)
        *(--result) = static_cast<byte>(carry);
}

template <class byte, class Traits = traits<byte>>
void Cry_subtract(byte* result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    typedef typename Traits::wide_type wide_t;
    wide_t carry = 0;

    --last1;
    --last2;

    for (; (last1 >= first1) && (last2 >= first2); --last1, --last2)
    {
        if (*(last1) < *(last2) + carry)
        {
            *(--result) = static_cast<byte>((*last1) - (*last2) - carry + Traits::base);
            carry       = 1;
        }
        else
        {
            *(--result) = static_cast<byte>((*last1) - (*last2) - carry);
            carry       = 0;
        }
    }

    for (; first1 <= last1; --last1)
    {
        *(--result) = *(last1)-carry;
        carry       = 0;
    }
}

template <class byte, class Traits = traits<byte>>
void Cry_multiply(byte* last_result, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    typedef typename Traits::wide_type wide_t;

    --last1;
    --last2;

    byte carry = 0x00;

    for (; first2 <= last2; --last2)
    {
        --last_result;

        byte* resultIter = last_result;

        for (const byte* last_1 = last1; first1 <= last_1; --last_1)
        {
            wide_t temp = static_cast<wide_t>(*resultIter) + static_cast<wide_t>(*last_1) * static_cast<wide_t>(*last2) + static_cast<wide_t>(carry);

            carry = temp / Traits::base;

            *(resultIter) = temp % Traits::base;

            --resultIter;
        }

        *(resultIter) = carry;
        carry         = 0x00;
    }
}

template <class byte, class Traits = traits<byte>>
void Cry_multiply(byte* last_result, const byte* first1, const byte* last1, byte x)
{
    typedef typename Traits::wide_type wide_t;

    --last1;
    --last_result;

    byte carry = 0x00;

    for (; first1 <= last1; --last1)
    {
        wide_t temp = static_cast<wide_t>(*last1) * static_cast<wide_t>(x) + static_cast<wide_t>(carry);

        carry          = temp / Traits::base;
        *(last_result) = temp % Traits::base;

        --last_result;
    }

    *last_result = carry;
}

template <class byte, class Traits = traits<byte>>
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
            return static_cast<typename Traits::wide_type>((*first1 < *first2) ? -1 : 1);
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

template <class byte, class Traits = traits<byte>>
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

template <class byte, class Traits = traits<byte>>
void Cry_div_rem(byte* div_last, byte* rem_last, const byte* first1, const byte* last1, const byte* first2, const byte* last2)
{
    typedef typename Traits::wide_type wide_t;

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

    auto cmp = Cry_compare<byte>(rFirst, rLast, dFirst, dLast);
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
        wide_t Up = Traits::base;

        auto cmp = Cry_compare<byte>(rFirst, rLast, dFirst, dLast);
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

template <class byte, class Traits = traits<byte>>
void Cry_increment(byte* first, byte* last)
{
    typedef typename Traits::wide_type wide_t;

    byte carry = 0x00;

    wide_t tmp = static_cast<wide_t>(*--last) + static_cast<wide_t>(0x01) + static_cast<wide_t>(carry);
    *(last)    = static_cast<byte>(tmp);
    carry      = tmp >> sizeof(carry) * 8;

    for (; (first <= last) && carry;)
    {
        tmp     = static_cast<wint_t>(*--last) + static_cast<wint_t>(carry);
        *(last) = static_cast<byte>(tmp);
        carry   = tmp >> sizeof(carry) * 8;
    }
}

template <class byte, class Traits = traits<byte>>
void Cry_decrement(byte* first, byte* last)
{
    typedef typename Traits::wide_type wide_t;
    wide_t carry = 0;

    if (*--last < 0x01)
    {
        *(last) = (*last) - (0x01) - carry + Traits::base;
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
            *(last) = (*last) - carry + Traits::base;
            carry   = 1;
        }
        else
        {
            *(last) = (*last) - carry;
            carry   = 0;
        }
    }
}

template <class byte, class Traits = traits<byte>>
bool Cry_is_odd(const byte* first, const byte* last)
{
    return (*(--last) & 0x01) == 0x01;
}

template <class byte, class Traits = traits<byte>>
bool Cry_is_even(const byte* first, const byte* last)
{
    return (*(--last) & 0x01) == 0x00;
}

template <class byte, class Traits = traits<byte>>
void Cry_reverse(byte* first, byte* last)
{
    std::reverse(first, last);
}

template <class byte, class Traits = traits<byte>>
void Cry_rotl(byte* first, byte* last)
{
    byte carry = (*first >> (sizeof(byte) * 8 - 1)) & 0x01;

    for (; first != last; --last)
    {
        int z = (*last >> 7) & 0x01;

        *last <<= 1;
        *last |= static_cast<byte>(carry);
        carry = z;
    }
}

template <class byte, class Traits = traits<byte>>
void Cry_rotl(byte* first, byte* last, int n)
{
    while (n--)
    {
        Cry_rotl(first, last);
    }
}

template <class byte, class Traits = traits<byte>>
void Cry_rotr(byte* first, byte* last)
{
    byte carry = *(last - 1) & 0x01;

    for (; first != last; ++first)
    {
        int z = *first & 0x01;

        *first >>= 1;
        *first |= static_cast<byte>(carry << (sizeof(byte) * 8 - 1));
        carry = z;
    }
}

template <class byte, class Traits = traits<byte>>
void Cry_rotr(byte* first, byte* last, int n)
{
    --last;

    while (n--)
    {
        Cry_rotr(first, last);
    }
}

template <class byte, class Traits = traits<byte>>
void Cry_rshift(byte* first, byte* last)
{
    byte carry = 0;

    for (; first != last; ++first)
    {
        int z = (*first) & 0x01;

        *first >>= 1;
        *first |= static_cast<byte>(carry << (sizeof(byte) * 8 - 1));
        carry = z;
    }
}

template <class byte, class Traits = traits<byte>>
void Cry_rshift(byte* first, byte* last, int n)
{
    while (n--)
    {
        Cry_rshift(first, last);
    }
}

template <class byte, class Traits = traits<byte>>
void Cry_lshift(byte* first, byte* last)
{
    byte carry = 0;

    --last;

    for (; first <= last; --last)
    {
        byte z = (*(last) >> (sizeof(byte) * 8 - 1)) & 0x01;

        *last <<= 1;
        *last |= static_cast<byte>(carry);
        carry = z;
    }
}

template <class byte, class Traits = traits<byte>>
void Cry_lshift(byte* first, byte* last, int n)
{
    while (n--)
    {
        Cry_lshift(first, last);
    }
}

template <class byte, class Traits = traits<byte>>
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

template <class byte, class Traits = traits<byte>>
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

template <class byte, class Traits = traits<byte>>
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

template <class byte, class Traits = traits<byte>>
void Cry_inverse(byte* first, byte* last)
{
    for (; first != last; ++first)
    {
        *first = ~(*first);
    }
}

template <class byte, class Traits = traits<byte>>
bool Cry_is_zero(const byte* first, const byte* last)
{
    for (; (first != last) && (*first == 0x00);)
    {
        ++first;
    }

    return first == last;
}

template <class byte, class Traits = traits<byte>>
int Cry_get_lowest_set_bit(const byte* first, const byte* last)
{
    return ((*std::prev(last)) & 0x01);
}

template <class byte, class Traits = traits<byte>>
int Cry_get_highest_set_bit(const byte* first, const byte* last)
{
    return (*first & 0x80);
}

template <class byte, class Traits = traits<byte>>
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

#endif
