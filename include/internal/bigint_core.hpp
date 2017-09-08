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
        using carry_type             = uint16_t;
        static const carry_type base = 0xff + 1; // 256;
    };

    template <>
    struct traits<uint16_t>
    {
        using carry_type             = uint32_t;
        static const carry_type base = 0xffff + 1; // 65536;
    };

    template <>
    struct traits<uint32_t>
    {
        using carry_type             = uint64_t;
        static const carry_type base = 4294967296LL;
    };
}

template <class T, class Traits = traits<T>, class InputIterator, class OutputIterator>
OutputIterator Cry_add(OutputIterator result, InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2)
{
    T carry                      = 0;
    typename Traits::ex_type tmp = 0;

    for (; (first1 != last1) && (first2 != last2);)
    {
        tmp         = *(--last1) + *(--last2) + carry;
        *(--result) = static_cast<T>(tmp);
        carry       = tmp >> sizeof(carry) * 8;
    }

    for (; first1 != last1;)
    {
        tmp         = *(--last1) + (carry);
        *(--result) = static_cast<T>(tmp);
        carry       = tmp >> sizeof(carry) * 8;
    }

    for (; first2 != last2;)
    {
        tmp         = *(--last2) + (carry);
        *(--result) = static_cast<T>(tmp);
        carry       = tmp >> sizeof(carry) * 8;
    }

    *(--result) = static_cast<T>(carry);

    return result;
}

template <class T, class Traits = traits<T>, class InputIterator, class OutputIterator>
OutputIterator Cry_subtract(OutputIterator result, InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2)
{
    T carry = 0;

    for (; (first1 != last1) && (first2 != last2);)
    {
        if (*(--last1) < *(--last2) + carry)
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

    for (; first1 != last1;)
    {
        *(--result) = *(--last1) - carry;
        carry       = 0;
    }

    return result;
}

template <class T, class Traits = traits<T>, class InputIterator, class OutputIterator>
OutputIterator Cry_multiply(OutputIterator result, InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2)
{
    T carry = 0x00;

    for (; first2 != last2;)
    {
        auto resultIter = --result;
        --last2;

        for (auto last_1 = last1; first1 != last_1;)
        {
            typename Traits::carry_type temp = (*resultIter) + *(--last_1) * (*last2) + carry;

            carry = static_cast<T>((temp) / Traits::base);

            *(resultIter) = static_cast<T>(temp % Traits::base);

            --resultIter;
        }

        *(--resultIter) = carry;
        carry           = 0x00;
    }

    return result;
}

#endif
