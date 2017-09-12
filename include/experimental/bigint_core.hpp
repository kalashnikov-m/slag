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
        tmp         = static_cast<wide_t>(*rfirst1) + static_cast<wide_t>(*rfirst2) + carry;
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
        *(result++) = *(rfirst1) - carry;
        carry       = 0;
    }
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
            typename Traits::wide_type temp = (*resultIter) + *(--last_1) * (*last2) + carry;

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
