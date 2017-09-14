/*
 * File:   algorithm.h
 * Author: max
 *
 * Created on 09.09.2017 г.,
 */

#ifndef BIGINT_CORE_H
#define BIGINT_CORE_H

#include <cstdint>

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

template <class T, class Traits = traits<T>, class InputIterator, class OutputIterator>
void Cry_add(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2, OutputIterator rlast)
{
    std::reverse_iterator<InputIterator> rfirst1(last1);
    std::reverse_iterator<InputIterator> rlast1(first1);

    std::reverse_iterator<InputIterator> rfirst2(last2);
    std::reverse_iterator<InputIterator> rlast2(first2);

    std::reverse_iterator<OutputIterator> result(rlast);

    T carry = 0;
    typedef typename Traits::wide_type wide_t;
    wide_t tmp = 0;

    for (; (rfirst1 != rlast1) && (rfirst2 != rlast2); ++rfirst1, ++rfirst2)
    {
        tmp         = static_cast<wide_t>(*rfirst1) + static_cast<wide_t>(*rfirst2) + static_cast<wide_t>(carry);
        *(result++) = static_cast<T>(tmp);
        carry       = tmp >> sizeof(carry) * 8;
    }

    for (; rfirst1 != rlast1; ++rfirst1)
    {
        tmp         = static_cast<wide_t>(*rfirst1) + static_cast<wide_t>(carry);
        *(result++) = static_cast<T>(tmp);
        carry       = tmp >> sizeof(carry) * 8;
    }

    for (; rfirst2 != rlast2; ++rfirst2)
    {
        tmp         = static_cast<wide_t>(*rfirst2) + static_cast<wide_t>(carry);
        *(result++) = static_cast<T>(tmp);
        carry       = tmp >> sizeof(carry) * 8;
    }

    if (carry)
        *(result++) = static_cast<T>(carry);
}

template <class T, class Traits = traits<T>, class InputIterator, class OutputIterator>
void Cry_subtract(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2, OutputIterator rlast)
{
    std::reverse_iterator<InputIterator> rfirst1(last1);
    std::reverse_iterator<InputIterator> rlast1(first1);

    std::reverse_iterator<InputIterator> rfirst2(last2);
    std::reverse_iterator<InputIterator> rlast2(first2);

    std::reverse_iterator<OutputIterator> result(rlast);

    T carry = 0;

    for (; (rfirst1 != rlast1) && (rfirst2 != rlast2); ++rfirst1, ++rfirst2)
    {
        if (*(rfirst1) < *(rfirst2) + carry)
        {
            *(result++) = static_cast<T>((*rfirst1) - (*rfirst2) - carry + Traits::base);
            carry       = 1;
        }
        else
        {
            *(result++) = static_cast<T>((*rfirst1) - (*rfirst2) - carry);
            carry       = 0;
        }
    }

    for (; rfirst1 != rlast1; ++rfirst1)
    {
        *(result++) = *(rfirst1)-carry;
        carry       = 0;
    }
}

template <class T, class Traits = traits<T>, class InputIterator, class OutputIterator>
void Cry_multiply(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2, OutputIterator rlast)
{
    std::reverse_iterator<InputIterator> rfirst1(last1);
    std::reverse_iterator<InputIterator> rlast1(first1);

    std::reverse_iterator<InputIterator> rfirst2(last2);
    std::reverse_iterator<InputIterator> rlast2(first2);

    std::reverse_iterator<OutputIterator> result(rlast);

    typedef typename Traits::wide_type wide_t;
    wide_t tmp = 0;

    T carry = 0x00;

    for (; rfirst2 != rlast2; ++rfirst2)
    {
        auto resultIter = result++;

        for (auto rfirst_1 = rfirst1; rfirst_1 != rlast1; ++rfirst_1)
        {
            tmp = static_cast<wide_t>(*resultIter) + static_cast<wide_t>(*rfirst_1) * static_cast<wide_t>(*rfirst2) + static_cast<wide_t>(carry);

            carry = static_cast<T>(tmp / Traits::base);

            *(resultIter++) = tmp % Traits::base;
        }

        *(resultIter++) = carry;
        carry           = 0x00;
    }
}

template <class T, class Traits = traits<T>, class InputIterator, class OutputIterator>
void Cry_multiply(InputIterator first, InputIterator last, T x, OutputIterator result_last)
{
    std::reverse_iterator<InputIterator> rfirst(last);
    std::reverse_iterator<InputIterator> rlast(first);
    std::reverse_iterator<OutputIterator> result(result_last);

    typedef typename Traits::wide_type wide_t;
    T carry = 0;

    for (; rfirst != rlast; ++rfirst)
    {
        wide_t temp = static_cast<wide_t>(*rfirst) * static_cast<wide_t>(x) + static_cast<wide_t>(carry);

        carry       = static_cast<T>(temp / Traits::base);
        *(result++) = temp % Traits::base;
    }

    *result++ = carry;
}

template <class T, class Traits = traits<T>, class InputIterator, class OutputIterator>
void Cry_div_rem(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2, OutputIterator div_last, OutputIterator rem_last)
{
    while ((first1 != last1) && (*first1 == 0x00))
    {
        ++first1;
    }

    while ((first2 != last2) && (*first2 == 0x00))
    {
        ++first2;
    }

    auto rTmp         = std::vector<T>(first1, last1);
    auto d1     = std::distance(first1, last1);
    auto d2     = std::distance(first2, last2);
    auto shift  = d1 - d2 + 1;
	auto rFirst = rTmp.begin();
    auto rLast  = rFirst + d2;;
    auto dFirst = first2;
    auto dLast  = last2;

    std::advance(div_last, -shift);

    auto cmp = Cry_compare<T>(rFirst, rLast, dFirst, dLast);
    if (cmp == -1)
    {
        ++rLast;
        ++div_last;
        --shift;
    }

    size_t nbytes = d2 + 1;
    std::vector<T> mul(nbytes);

    typedef typename Traits::wide_type wide_t;

    while (shift > 0)
    {
        T Down    = 0;
        wide_t Up = Traits::base;

        auto cmp = Cry_compare<T>(rFirst, rLast, dFirst, dLast);
        if (cmp == -1)
        {
            ++rLast;
        }

        for (; Down < Up - 1;)
        {
            // 1. c <-- (down + up) / 2;
            T Middle = static_cast<T>((Down + Up) / 2);

            // 2. mul <-- d * c;
            std::fill(mul.begin(), mul.end(), 0x00);

            Cry_multiply<T>(dFirst, dLast, Middle, mul.end());

            short mulCmp = Cry_compare<T>(mul.begin(), mul.end(), rFirst, rLast);

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
                Down = static_cast<T>(Up);
            }
        }

        std::fill(mul.begin(), mul.end(), 0x00);

        Cry_multiply(dFirst, dLast, Down, mul.end());

        Cry_subtract<T>(rFirst, rLast, mul.begin(), mul.end(), rLast);

        *(div_last++) = Down;

        --shift;
    }

    std::copy_backward(rFirst, rLast, rem_last);
}

template <class T, class Traits = traits<T>, class InputIterator>
void Cry_increment(InputIterator first, InputIterator last)
{
    typedef typename Traits::wide_type wide_t;

    T carry = 0x00;

    std::reverse_iterator<InputIterator> rfirst(last);
    std::reverse_iterator<InputIterator> rlast(first);

    wide_t tmp  = static_cast<wide_t>(*rfirst) + 0x01 + static_cast<wide_t>(carry);
    *(rfirst++) = static_cast<T>(tmp);
    carry       = tmp >> sizeof(carry) * 8;

    for (; (rfirst != rlast) && carry; ++rfirst)
    {
        tmp       = static_cast<wide_t>(*rfirst) + static_cast<wide_t>(carry);
        *(rfirst) = static_cast<T>(tmp);
        carry     = tmp >> sizeof(carry) * 8;
    }
}

template <class T, class Traits = traits<T>, class InputIterator>
void Cry_decrement(InputIterator first, InputIterator last)
{
    typedef typename Traits::wide_type wide_t;

    T carry = 0x00;

    std::reverse_iterator<InputIterator> rfirst(last);
    std::reverse_iterator<InputIterator> rlast(first);

    if (*rfirst < 0x01)
    {
        *(rfirst) = static_cast<T>(static_cast<wide_t>(*rfirst) - static_cast<wide_t>(0x01) - static_cast<wide_t>(carry) + static_cast<wide_t>(Traits::base));
        carry     = 1;
    }
    else
    {
        *(rfirst) = static_cast<T>(static_cast<wide_t>(*rfirst) - static_cast<wide_t>(0x01) - static_cast<wide_t>(carry));
        carry     = 0;
    }

    ++rfirst;

    for (; (rfirst != rlast && carry); ++rfirst)
    {
        if (*rfirst < carry)
        {
            *(rfirst) = static_cast<T>(static_cast<wide_t>(*rfirst) - static_cast<wide_t>(carry) + static_cast<wide_t>(Traits::base));
            carry     = 1;
        }
        else
        {
            *(rfirst) = static_cast<T>(static_cast<wide_t>(*rfirst) - static_cast<wide_t>(carry));
            carry     = 0;
        }
    }
}

template <class T, class Traits = traits<T>, class InputIterator>
bool Cry_is_odd(InputIterator first, InputIterator last)
{
    std::reverse_iterator<InputIterator> rfirst(last);

    return (*rfirst & 0x01) == 0x01;
}

template <class T, class Traits = traits<T>, class InputIterator>
bool Cry_is_even(InputIterator first, InputIterator last)
{
    std::reverse_iterator<InputIterator> rfirst(last);

    return (*rfirst & 0x01) == 0x00;
}

template <class T, class Traits = traits<T>, class InputIterator>
void Cry_rotl(InputIterator first, InputIterator last)
{
    std::reverse_iterator<InputIterator> rfirst(last);
    std::reverse_iterator<InputIterator> rlast(first);

    T carry = (*(rlast - 1) >> (sizeof(T) * 8 - 1)) & 0x01;

    for (; rfirst != rlast; ++rfirst)
    {
        T z = (*rfirst >> (sizeof(T) * 8 - 1)) & 0x01;

        *rfirst <<= 1;
        *rfirst |= static_cast<T>(carry);
        carry = z;
    }
}

template <class T, class Traits = traits<T>, class InputIterator>
void Cry_rotr(InputIterator first, InputIterator last)
{
    std::reverse_iterator<InputIterator> rfirst(last);

    T carry = *(rfirst)&0x01;

    for (; first != last; ++first)
    {
        T z = *first & 0x01;

        *first >>= 1;
        *first |= static_cast<T>(carry << (sizeof(T) * 8 - 1));
        carry = z;
    }
}

template <class T, class Traits = traits<T>, class InputIterator>
void Cry_rshift(InputIterator first, InputIterator last)
{
    T carry = 0;

    for (; first != last; ++first)
    {
        T z = (*first) & 0x01;

        *first >>= 1;
        *first |= static_cast<T>(carry << (sizeof(T) * 8 - 1));
        carry = z;
    }
}

template <class T, class Traits = traits<T>, class InputIterator>
void Cry_lshift(InputIterator first, InputIterator last)
{
    std::reverse_iterator<InputIterator> rfirst(last);
    std::reverse_iterator<InputIterator> rlast(first);

    T carry = 0;

    for (; rfirst != rlast; ++rfirst)
    {
        T z = (*rfirst >> (sizeof(T) * 8 - 1)) & 0x01;

        *rfirst <<= 1;
        *rfirst |= static_cast<T>(carry);
        carry = z;
    }
}

template <class T, class Traits = traits<T>, class InputIterator>
void Cry_rotl(InputIterator first, InputIterator last, int n)
{
    while (n--)
    {
        Cry_rotl(first, last);
    }
}

template <class T, class Traits = traits<T>, class InputIterator>
void Cry_rotr(InputIterator first, InputIterator last, int n)
{
    --last;

    while (n--)
    {
        Cry_rotr(first, last);
    }
}

template <class T, class Traits = traits<T>, class InputIterator>
void Cry_lshift(InputIterator first, InputIterator last, int n)
{
    while (n--)
    {
        Cry_lshift(first, last);
    }
}

template <class T, class Traits = traits<T>, class InputIterator>
void Cry_rshift(InputIterator first, InputIterator last, int n)
{
    while (n--)
    {
        Cry_rshift(first, last);
    }
}

template <class T, class Traits = traits<T>, class InputIterator, class OutputIterator>
void Cry_or(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2, OutputIterator rlast)
{
    std::reverse_iterator<InputIterator> rfirst1(last1);
    std::reverse_iterator<InputIterator> rlast1(first1);

    std::reverse_iterator<InputIterator> rfirst2(last2);
    std::reverse_iterator<InputIterator> rlast2(first2);

    std::reverse_iterator<OutputIterator> result(rlast);

    for (; (rfirst1 != rlast1) && (rfirst2 != rlast2); ++rfirst1, ++rfirst2)
    {
        *(result++) = *(rfirst1) | *(rfirst2);
    }

    for (; rfirst1 != rlast1; ++rfirst1)
    {
        *(result++) = *rfirst1;
    }

    for (; rfirst2 != rlast2; ++rfirst2)
    {
        *(result++) = *rfirst2;
    }
}

template <class T, class Traits = traits<T>, class InputIterator, class OutputIterator>
void Cry_xor(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2, OutputIterator rlast)
{
    std::reverse_iterator<InputIterator> rfirst1(last1);
    std::reverse_iterator<InputIterator> rlast1(first1);

    std::reverse_iterator<InputIterator> rfirst2(last2);
    std::reverse_iterator<InputIterator> rlast2(first2);

    std::reverse_iterator<OutputIterator> result(rlast);

    for (; (rfirst1 != rlast1) && (rfirst2 != rlast2); ++rfirst1, ++rfirst2)
    {
        *(result++) = *(rfirst1) ^ *(rfirst2);
    }

    for (; rfirst1 != rlast1; ++rfirst1)
    {
        *(result++) = *rfirst1;
    }

    for (; rfirst2 != rlast2; ++rfirst2)
    {
        *(result++) = *rfirst2;
    }
}

template <class T, class Traits = traits<T>, class InputIterator, class OutputIterator>
void Cry_and(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2, OutputIterator rlast)
{
    std::reverse_iterator<InputIterator> rfirst1(last1);
    std::reverse_iterator<InputIterator> rlast1(first1);

    std::reverse_iterator<InputIterator> rfirst2(last2);
    std::reverse_iterator<InputIterator> rlast2(first2);

    std::reverse_iterator<OutputIterator> result(rlast);

    for (; (rfirst1 != rlast1) && (rfirst2 != rlast2); ++rfirst1, ++rfirst2)
    {
        *(result++) = *(rfirst1) & *(rfirst2);
    }

    for (; rfirst1 != rlast1; ++rfirst1)
    {
        *(result++) = 0;
    }

    for (; rfirst2 != rlast2; ++rfirst2)
    {
        *(result++) = 0;
    }
}

template <class T, class Traits = traits<T>, class InputIterator>
void Cry_inverse(InputIterator first, InputIterator last)
{
    for (; first != last; ++first)
    {
        *first = ~(*first);
    }
}

template <class T, class Traits = traits<T>, class InputIterator>
short Cry_compare(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2)
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

template <class T, class Traits = traits<T>, class InputIterator>
bool Cry_equal(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2)
{

    for (; (first1 != last1) && (*first1 == 0x00); ++first1)
        ;

    for (; (first2 != last2) && (*first2 == 0x00); ++first2)
        ;

    for (; (first1 != last1) && (first2 != last2) && (*first1) == (*first2); ++first1, ++first2)
        ;

    return (first1 == last1) && (first2 == last2);
}

template <class T, class Traits = traits<T>, class InputIterator>
bool Cry_is_zero(InputIterator first, InputIterator last)
{
    for (; (first != last) && (*first == 0x00);)
    {
        ++first;
    }

    return first == last;
}

template <class T, class Traits = traits<T>, class InputIterator>
int Cry_get_lowest_set_bit(InputIterator first, InputIterator last)
{
    return ((*std::prev(last)) & 0x01);
}

template <class T, class Traits = traits<T>, class InputIterator>
int Cry_get_highest_set_bit(InputIterator first, InputIterator last)
{
    return (*first & 0x80);
}

template <class T, class Traits = traits<T>, class InputIterator>
bool Cry_is_power_of_two(InputIterator first, InputIterator last)
{
    return false;
}

template <class T, class Traits = traits<T>, class InputIterator>
bool Cry_is_one(InputIterator first, InputIterator last)
{
    std::reverse_iterator<InputIterator> rfirst(last);
    std::reverse_iterator<InputIterator> rlast(first);

    if (*rfirst == 0x01)
    {
        ++rfirst;

        for (; (rfirst != rlast) && (*rfirst == 0x00);)
        {
            ++rfirst;
        }

        return (rfirst == rlast);
    }

    return false;
}

#endif
