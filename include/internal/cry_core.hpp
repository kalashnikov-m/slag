
#ifndef CRY_CORE_HPP
#define CRY_CORE_HPP

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

template <class T, class Traits = traits<T>>
void Cry_add(T* result, const T* first1, const T* last1, const T* first2, const T* last2)
{
    T carry = 0;
    typedef typename Traits::wide_type wide_t;
    wide_t tmp = 0;

    --last1;
    --last2;

    for (; (first1 <= last1) && (first2 <= last2); --last1, --last2)
    {
        tmp         = static_cast<wide_t>(*last1) + static_cast<wide_t>(*last2) + static_cast<wide_t>(carry);
        *(--result) = static_cast<T>(tmp);
        carry       = tmp >> sizeof(carry) * 8;
    }

    for (; first1 <= last1; --last1)
    {
        tmp         = static_cast<wide_t>(*last1) + static_cast<wide_t>(carry);
        *(--result) = static_cast<T>(tmp);
        carry       = tmp >> sizeof(carry) * 8;
    }

    for (; first2 <= last2; --last2)
    {
        tmp         = static_cast<wide_t>(*last2) + static_cast<wide_t>(carry);
        *(--result) = static_cast<T>(tmp);
        carry       = tmp >> sizeof(carry) * 8;
    }

    if (carry)
        *(--result) = static_cast<T>(carry);
}

template <class T, class Traits = traits<T>>
void Cry_subtract(T* result, const T* first1, const T* last1, const T* first2, const T* last2)
{
    typedef typename Traits::wide_type wide_t;
    T carry = 0;

    --last1;
    --last2;

    for (; (last1 >= first1) && (last2 >= first2); --last1, --last2)
    {
        if (*(last1) < *(last2) + carry)
        {
            *(--result) = static_cast<T>((*last1) - (*last2) - carry + Traits::base);
            carry       = 1;
        }
        else
        {
            *(--result) = static_cast<T>((*last1) - (*last2) - carry);
            carry       = 0;
        }
    }

    for (; first1 <= last1; --last1)
    {
        *(--result) = *(last1)-carry;
        carry       = 0;
    }
}

template <class T, class Traits = traits<T>>
void Cry_multiply(T* last_result, const T* first1, const T* last1, const T* first2, const T* last2)
{
    typedef typename Traits::wide_type wide_t;

    --last1;
    --last2;

    T carry = 0x00;

    for (; first2 <= last2; --last2)
    {
        --last_result;

        T* resultIter = last_result;

        for (const T* last_1 = last1; first1 <= last_1; --last_1)
        {
            wide_t temp = static_cast<wide_t>(*resultIter) + static_cast<wide_t>(*last_1) * static_cast<wide_t>(*last2) + static_cast<wide_t>(carry);

            carry = static_cast<T>(temp / Traits::base);

            *(resultIter) = temp % Traits::base;

            --resultIter;
        }

        *(resultIter) = carry;
        carry         = 0x00;
    }
}

template <class T, class Traits = traits<T>>
void Cry_multiply(T* last_result, const T* first1, const T* last1, T x)
{
    typedef typename Traits::wide_type wide_t;

    --last1;
    --last_result;

    T carry = 0x00;

    for (; first1 <= last1; --last1)
    {
        wide_t temp = static_cast<wide_t>(*last1) * static_cast<wide_t>(x) + static_cast<wide_t>(carry);

        carry          = static_cast<T>(temp / Traits::base);
        *(last_result) = temp % Traits::base;

        --last_result;
    }

    *last_result = carry;
}

template <class T, class Traits = traits<T>>
short Cry_compare(const T* first1, const T* last1, const T* first2, const T* last2)
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

    auto dA = last1 - first1;
    auto dB = last2 - first2;

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

template <class T, class Traits = traits<T>>
bool Cry_equal(const T* first1, const T* last1, const T* first2, const T* last2)
{

    for (; (first1 != last1) && (*first1 == 0x00); ++first1)
        ;

    for (; (first2 != last2) && (*first2 == 0x00); ++first2)
        ;

    for (; (first1 != last1) && (first2 != last2) && (*first1) == (*first2); ++first1, ++first2)
        ;

    return (first1 == last1) && (first2 == last2);
}

template <class T, class Traits = traits<T>>
void Cry_divide(T* div_last, T* rem_last, const T* first1, const T* last1, const T* first2, const T* last2)
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

    auto rTmp = std::vector<T>(first1, last1);

    auto d1         = last1 - first1;
    auto d2         = last2 - first2;
    auto shift      = d1 - d2 + 1;
    T* rFirst       = &rTmp[0];
    T* rLast        = rFirst + d2;
    const T* dFirst = first2;
    const T* dLast  = last2;

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

    while (shift > 0)
    {
        T Down    = 0x00;
        wide_t Up = Traits::base;

        cmp = Cry_compare<T>(rFirst, rLast, dFirst, dLast);
        if (cmp == -1)
        {
            ++rLast;
        }

        for (; Down < Up - 1;)
        {
            // 1. c <-- (down + up) / 2;
            T Middle = static_cast<T>(((Down + Up) / 2));

            // 2. mul <-- d * c;
            std::fill(&mul[0], &mul[0] + nbytes, 0x00);

            Cry_multiply(&mul[0] + nbytes, dFirst, dLast, Middle);

            const short mulCmp = Cry_compare(&mul[0], &mul[0] + nbytes, rFirst, rLast);

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

        std::fill(&mul[0], &mul[0] + nbytes, 0x00);

        Cry_multiply(&mul[0] + nbytes, dFirst, dLast, Down);

        Cry_subtract(rLast, rFirst, rLast, &mul[0], &mul[0] + nbytes);

        *(div_last++) = Down;

        --shift;
    }

    std::copy_backward(rFirst, rLast, rem_last);
}

template <class T, class Traits = traits<T>>
void Cry_increment(T* first, T* last)
{
    typedef typename Traits::wide_type wide_t;

    T carry = 0x00;

    wide_t tmp = static_cast<wide_t>(*--last) + static_cast<wide_t>(0x01) + static_cast<wide_t>(carry);
    *(last)    = static_cast<T>(tmp);
    carry      = tmp >> sizeof(carry) * 8;

    for (; (first <= last) && carry;)
    {
        tmp     = static_cast<wint_t>(*--last) + static_cast<wint_t>(carry);
        *(last) = static_cast<T>(tmp);
        carry   = tmp >> sizeof(carry) * 8;
    }
}

template <class T, class Traits = traits<T>>
void Cry_decrement(T* first, T* last)
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

template <class T, class Traits = traits<T>>
bool Cry_is_odd(const T* first, const T* last)
{
    return (*(--last) & 0x01) == 0x01;
}

template <class T, class Traits = traits<T>>
bool Cry_is_even(const T* first, const T* last)
{
    return (*(--last) & 0x01) == 0x00;
}

template <class T, class Traits = traits<T>>
void Cry_reverse(T* first, T* last)
{
    std::reverse(first, last);
}

template <class T, class Traits = traits<T>>
void Cry_rotl(T* first, T* last)
{
    T carry = (*first >> (sizeof(T) * 8 - 1)) & 0x01;

    for (; first != last; --last)
    {
        const int z = (*last >> 7) & 0x01;

        *last <<= 1;
        *last |= static_cast<T>(carry);
        carry = z;
    }
}

template <class T, class Traits = traits<T>>
void Cry_rotl(T* first, T* last, int n)
{
    while (n--)
    {
        Cry_rotl(first, last);
    }
}

template <class T, class Traits = traits<T>>
void Cry_rotr(T* first, T* last)
{
    T carry = *(last - 1) & 0x01;

    for (; first != last; ++first)
    {
        const int z = *first & 0x01;

        *first >>= 1;
        *first |= static_cast<T>(carry << (sizeof(T) * 8 - 1));
        carry = z;
    }
}

template <class T, class Traits = traits<T>>
void Cry_rotr(T* first, T* last, int n)
{
    --last;

    while (n--)
    {
        Cry_rotr(first, last);
    }
}

template <class T, class Traits = traits<T>>
void Cry_rshift(T* first, T* last)
{
    T carry = 0;

    for (; first != last; ++first)
    {
        const int z = (*first) & 0x01;

        *first >>= 1;
        *first |= static_cast<T>(carry << (sizeof(T) * 8 - 1));
        carry = z;
    }
}

template <class T, class Traits = traits<T>>
void Cry_rshift(T* first, T* last, int n)
{
    while (n--)
    {
        Cry_rshift(first, last);
    }
}

template <class T, class Traits = traits<T>>
void Cry_lshift(T* first, T* last)
{
    T carry = 0;

    --last;

    for (; first <= last; --last)
    {
        T z = (*(last) >> (sizeof(T) * 8 - 1)) & 0x01;

        *last <<= 1;
        *last |= static_cast<T>(carry);
        carry = z;
    }
}

template <class T, class Traits = traits<T>>
void Cry_lshift(T* first, T* last, int n)
{
    while (n--)
    {
        Cry_lshift(first, last);
    }
}

template <class T, class Traits = traits<T>>
void Cry_or(T* result, const T* first1, const T* last1, const T* first2, const T* last2)
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

template <class T, class Traits = traits<T>>
void Cry_xor(T* result, const T* first1, const T* last1, const T* first2, const T* last2)
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

template <class T, class Traits = traits<T>>
void Cry_and(T* result, const T* first1, const T* last1, const T* first2, const T* last2)
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

template <class T, class Traits = traits<T>>
void Cry_inverse(T* first, T* last)
{
    for (; first != last; ++first)
    {
        *first = ~(*first);
    }
}

template <class T, class Traits = traits<T>>
bool Cry_is_zero(const T* first, const T* last)
{
    for (; (first != last) && (*first == 0x00);)
    {
        ++first;
    }

    return first == last;
}

template <class T, class Traits = traits<T>>
int Cry_get_lowest_set_bit(const T* first, const T* last)
{
    return ((*std::prev(last)) & 0x01);
}

template <class T, class Traits = traits<T>>
int Cry_get_highest_set_bit(const T* first, const T* last)
{
    return (*first & 0x80);
}

template <class T, class Traits = traits<T>>
bool Cry_is_one(const T* first, const T* last)
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
