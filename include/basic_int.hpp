
#ifndef BASIC_INT_HPP
#define BASIC_INT_HPP

#include <algorithm>
#include <initializer_list>
#include <sstream>
#include <vector>

#include <cctype>
#include <cstdint>

#include "encoders.hpp"
#include <cry_core.hpp>

using namespace std;

namespace cry
{
    template <class IntType>
    class basic_int
    {
      public:
        basic_int() : m_Polynomial(1), m_Negative(false)
        {
        }

        ~basic_int()
        {
        }

        constexpr basic_int(const basic_int& other) : m_Polynomial(other.m_Polynomial), m_Negative(other.m_Negative)
        {
        }

        basic_int(basic_int&& other) noexcept
        {
            *this = std::move(other);
        }

        basic_int(const std::initializer_list<IntType>& il, bool negative = false) : basic_int(std::begin(il), std::end(il), negative)
        {
        }

        basic_int(const std::vector<IntType>& iv, bool negative = false) : basic_int(std::begin(iv), std::end(iv), negative)
        {
        }

        basic_int(uint32_t x) : basic_int({static_cast<IntType>((x & 0xff000000) >> 24), static_cast<IntType>((x & 0x00ff0000) >> 16), static_cast<IntType>((x & 0x0000ff00) >> 8), static_cast<IntType>(x & 0x000000ff)})
        {
        }

        basic_int(const std::string& hex) : m_Polynomial(1), m_Negative(false)
        {
            if (hex.empty())
            {
                return;
            }

            /////////////////////////////////
            // skiping zeros and whitespaces
            auto it(hex.begin());
            for (; it != hex.end() && ((*it == '0') || (*it == ' '));)
                ++it;

            std::string::const_reverse_iterator rit(hex.rbegin()), rend(it);

            //////////////////////////////////
            // counting hexadecimal characters
            const size_t nchars = std::count_if(rit, rend, [](unsigned char c) { return std::isalnum(c); });
            size_t nbytes       = nchars / 2;
            nbytes += nchars % 2;

            size_t nwords = nbytes / sizeof(IntType);
            if (nbytes % sizeof(IntType))
                nwords += 1;

            IntType word = 0;
            size_t cnt   = 0;

            m_Polynomial.resize(nwords);
            auto ret(m_Polynomial.rbegin());

            for (; rit != rend; ++rit)
            {
                uint8_t bt = *rit;

                // transform hex character to the 4bit equivalent number, using the ascii table indexes
                if (bt >= '0' && bt <= '9')
                    bt = bt - '0';
                else if (bt >= 'a' && bt <= 'f')
                    bt = bt - 'a' + 10;
                else if (bt >= 'A' && bt <= 'F')
                    bt = bt - 'A' + 10;
                else if (bt == ' ')
                    continue;
                else
                    throw std::logic_error("Invalid hex string. Only hex numbers are allowed.");

                // shift 4 to make space for new digit, and add the 4 bits of the new digit
                // word = (word << 4) | (bt & 0xF);
                word = (bt << cnt * 4) | (word);

                ++cnt;

                if (cnt == sizeof(IntType) * 2)
                {
                    *ret++ = word;
                    word   = 0;
                    cnt    = 0;
                }
            }

            if (ret != m_Polynomial.rend())
                *ret++ = word;
        }

        template <class InputIterator>
        basic_int(InputIterator first, InputIterator last, bool negative = false) : m_Polynomial(1), m_Negative(false)
        {
            for (; first != last && *first == 0x00; ++first)
                ;

            if (first != last)
            {
                m_Polynomial = std::vector<IntType>(first, last);
                m_Negative   = negative;
            }
        }

        std::vector<IntType> polynomial() const
        {
            return m_Polynomial;
        }

        basic_int& operator=(const basic_int& other);

        basic_int& operator=(basic_int&& other) noexcept;

        explicit operator bool() const;

        const basic_int operator<<(int) const;

        const basic_int operator>>(int) const;

        basic_int& operator<<=(int nbits);

        basic_int& operator>>=(int nbits);

        basic_int& operator++();

        basic_int& operator--();

        basic_int& operator+();

        const basic_int operator++(int);

        const basic_int operator--(int);

        const basic_int operator-() const;

        const basic_int operator~() const;

        basic_int& operator+=(const basic_int& rhs);

        basic_int& operator-=(const basic_int& rhs);

        basic_int& operator%=(const basic_int& rhs);

        basic_int& operator&=(const basic_int& rhs);

        basic_int& operator|=(const basic_int& rhs);

        basic_int& operator^=(const basic_int& rhs);

        basic_int& operator*=(const basic_int& rhs);

        basic_int& operator/=(const basic_int& rhs);

        template <class X>
        friend ostream& operator<<(ostream& stream, const basic_int<X>& huge);

        friend bool operator&&(const basic_int& lhs, const basic_int& rhs)
        {
            const auto f1 = static_cast<bool>(lhs);
            const auto f2 = static_cast<bool>(rhs);

            return (f1 && f2);
        }

        friend bool operator||(const basic_int& lhs, const basic_int& rhs)
        {
            const auto f1 = static_cast<bool>(lhs);
            const auto f2 = static_cast<bool>(rhs);

            return (f1 || f2);
        }

        friend bool operator<(const basic_int& lhs, const basic_int& rhs)
        {
            const short cmp = compare(lhs, rhs);

            return (cmp == -1);
        }

        friend bool operator>(const basic_int& lhs, const basic_int& rhs)
        {
            return rhs < lhs;
        }

        friend bool operator<=(const basic_int& lhs, const basic_int& rhs)
        {
            return !(lhs > rhs);
        }

        friend bool operator>=(const basic_int& lhs, const basic_int& rhs)
        {
            return !(lhs < rhs);
        }

        friend bool operator==(const basic_int& lhs, const basic_int& rhs)
        {
            const short cmp = compare(lhs, rhs);

            return (cmp == 0);
        }

        friend bool operator!=(const basic_int& lhs, const basic_int& rhs)
        {
            return !(lhs == rhs);
        }

        friend const basic_int operator&(const basic_int& lhs, const basic_int& rhs)
        {
            const auto& a = lhs.m_Polynomial;
            const auto& b = rhs.m_Polynomial;

            auto max = std::max(a.size(), b.size());

            std::vector<IntType> out(max);

            Cry_and(&out[0] + out.size(), &lhs.m_Polynomial[0], &lhs.m_Polynomial[0] + lhs.m_Polynomial.size(), &rhs.m_Polynomial[0], &rhs.m_Polynomial[0] + rhs.m_Polynomial.size());

            return basic_int(out);
        }

        friend const basic_int operator|(const basic_int& lhs, const basic_int& rhs)
        {
            const auto& a = lhs.m_Polynomial;
            const auto& b = rhs.m_Polynomial;

            auto max = std::max(a.size(), b.size());

            std::vector<IntType> out(max);

            Cry_or(&out[0] + out.size(), &lhs.m_Polynomial[0], &lhs.m_Polynomial[0] + lhs.m_Polynomial.size(), &rhs.m_Polynomial[0], &rhs.m_Polynomial[0] + rhs.m_Polynomial.size());

            return basic_int(out);
        }

        friend const basic_int operator^(const basic_int& lhs, const basic_int& rhs)
        {
            const auto& a = lhs.m_Polynomial;
            const auto& b = rhs.m_Polynomial;

            auto max = std::max(a.size(), b.size());

            std::vector<IntType> out(max);

            Cry_xor(&out[0] + out.size(), &lhs.m_Polynomial[0], &lhs.m_Polynomial[0] + lhs.m_Polynomial.size(), &rhs.m_Polynomial[0], &rhs.m_Polynomial[0] + rhs.m_Polynomial.size());

            return basic_int(out);
        }

        friend const basic_int operator+(const basic_int& lhs, const basic_int& rhs)
        {
            const auto& a = lhs.m_Polynomial;
            const auto& b = rhs.m_Polynomial;

            const size_t lsize = a.size();
            const size_t rsize = b.size();

            std::vector<IntType> out(std::max(lsize, rsize));

            // если знаки аргументов различны: (a)+(-b), (-a)+(b) ==> ?(a-b)
            if (lhs.m_Negative ^ rhs.m_Negative)
            {
                const short cmp = Cry_compare(&a[0], &a[0] + lsize, &b[0], &b[0] + rsize);

                if (cmp == -1)
                { // (|a| < |b|) ==> (|b| - |a|)
                    Cry_subtract(&out[0] + out.size(), &b[0], &b[0] + rsize, &a[0], &a[0] + lsize);

                    return basic_int(out, rhs.m_Negative);
                }
                else if (cmp == +1)
                {
                    // (|a| > |b|) ==> (|a| - |b|)
                    Cry_subtract(&out[0] + out.size(), &a[0], &a[0] + lsize, &b[0], &b[0] + rsize);

                    return basic_int(out, lhs.m_Negative);
                }
            }
            else
            { // если знаки аргументов одинаковы
                Cry_add(&out[0] + out.size(), &a[0], &a[0] + lsize, &b[0], &b[0] + rsize);

                return basic_int<IntType>(out, lhs.m_Negative & rhs.m_Negative);
            }

            return basic_int();
        }

        friend const basic_int operator-(const basic_int& lhs, const basic_int& rhs)
        {
            const short cmp = Cry_compare(&lhs.m_Polynomial[0], &lhs.m_Polynomial[0] + lhs.m_Polynomial.size(), &rhs.m_Polynomial[0], &rhs.m_Polynomial[0] + rhs.m_Polynomial.size());

            // если знаки аргументов различны: (a)-(-b), (-a)-(b) ==> ?(a+b)
            if (lhs.m_Negative ^ rhs.m_Negative)
            {
                basic_int temp((cmp == -1) ? rhs : lhs);

                Cry_add(&temp.m_Polynomial[0] + temp.m_Polynomial.size(), &lhs.m_Polynomial[0], &lhs.m_Polynomial[0] + lhs.m_Polynomial.size(), &rhs.m_Polynomial[0], &rhs.m_Polynomial[0] + rhs.m_Polynomial.size());

                temp.m_Negative = lhs.m_Negative;

                return temp;
            }

            // если знаки аргументов одинаковы:
            if (cmp == -1)
            { // (|a| < |b|) ==> (|b| - |a|)
                basic_int temp(rhs);

                Cry_subtract(&temp.m_Polynomial[0] + temp.m_Polynomial.size(), &rhs.m_Polynomial[0], &rhs.m_Polynomial[0] + rhs.m_Polynomial.size(), &lhs.m_Polynomial[0], &lhs.m_Polynomial[0] + lhs.m_Polynomial.size());

                temp.m_Negative = (!lhs.m_Negative & !rhs.m_Negative);

                return temp;
            }
            if (cmp == +1)
            {
                // (|a| > |b|) ==> (|a| - |b|)
                basic_int temp(lhs);

                Cry_subtract(&temp.m_Polynomial[0] + temp.m_Polynomial.size(), &lhs.m_Polynomial[0], &lhs.m_Polynomial[0] + lhs.m_Polynomial.size(), &rhs.m_Polynomial[0], &rhs.m_Polynomial[0] + rhs.m_Polynomial.size());

                temp.m_Negative = (lhs.m_Negative & rhs.m_Negative);

                return temp;
            }

            return basic_int();
        }

        friend const basic_int operator*(const basic_int& lhs, const basic_int& rhs)
        {
            const auto& a = lhs.m_Polynomial;
            const auto& b = rhs.m_Polynomial;

            const size_t l_size = a.size();
            const size_t r_size = b.size();

            std::vector<IntType> out(l_size + r_size);

            Cry_multiply(&out[0] + out.size(), &a[0], &a[0] + l_size, &b[0], &b[0] + r_size);

            return basic_int(out, lhs.m_Negative ^ rhs.m_Negative);
        }

        friend const basic_int operator/(const basic_int& lhs, const basic_int& rhs)
        {
            basic_int div;
            basic_int rem;

            lhs.DivRem(div, rem, rhs);

            return div;
        }

        friend const basic_int operator%(const basic_int& lhs, const basic_int& rhs)
        {
            basic_int div;
            basic_int rem;

            lhs.DivRem(div, rem, rhs);

            return rem;
        }

        void DivRem(basic_int& q, basic_int& r, const basic_int& other) const;

      protected:
        void __swap(basic_int& other) noexcept
        {
            m_Polynomial.swap(other.m_Polynomial);

            std::swap(m_Negative, other.m_Negative);
        }

        template <class X>
        friend short compare(const basic_int<X>& lhs, const basic_int<X>& rhs);

      private:
        std::vector<IntType> m_Polynomial;
        bool m_Negative;
    };

    // =================================================================================

    template <class X>
    basic_int<X>& basic_int<X>::operator=(const basic_int<X>& other)
    {
        if (this != &other)
        {
            auto temp(other);

            this->__swap(temp);
        }

        return *this;
    }

    template <class X>
    basic_int<X>& basic_int<X>::operator=(basic_int<X>&& other) noexcept
    {
        if (this != &other)
        {
            m_Polynomial = std::move(other.m_Polynomial);
            m_Negative   = std::move(other.m_Negative);
        }

        return *this;
    }

    template <class X>
    basic_int<X>::operator bool() const
    {
        const bool flag = Cry_is_zero(&m_Polynomial[0], (&m_Polynomial[0] + m_Polynomial.size()));

        return !flag;
    }

    template <class X>
    basic_int<X>& basic_int<X>::operator++()
    {
        Cry_increment(&m_Polynomial[0], &m_Polynomial[0] + m_Polynomial.size());

        return *this;
    }

    template <class X>
    basic_int<X>& basic_int<X>::operator--()
    {
        Cry_decrement(&m_Polynomial[0], &m_Polynomial[0] + m_Polynomial.size());

        return *this;
    }

    template <class X>
    const basic_int<X> basic_int<X>::operator++(int)
    {
        basic_int temp(*this);

        ++(*this);

        return temp;
    }

    template <class X>
    const basic_int<X> basic_int<X>::operator--(int)
    {
        basic_int temp(*this);

        --(*this);

        return temp;
    }

    template <class X>
    basic_int<X>& basic_int<X>::operator+=(const basic_int<X>& rhs)
    {
        *this = *this + rhs;

        return *this;
    }

    template <class X>
    basic_int<X>& basic_int<X>::operator-=(const basic_int<X>& rhs)
    {
        *this = *this - rhs;

        return *this;
    }

    template <class X>
    basic_int<X>& basic_int<X>::operator+()
    {
        return (*this);
    }

    template <class X>
    const basic_int<X> basic_int<X>::operator-() const
    {
        return basic_int(m_Polynomial, !m_Negative);
    }

    template <class X>
    const basic_int<X> basic_int<X>::operator~() const
    {
        basic_int temp(*this);

        Cry_inverse(&temp.m_Polynomial[0], &temp.m_Polynomial[0] + temp.m_Polynomial.size());

        return temp;
    }

    template <class X>
    basic_int<X>& basic_int<X>::operator%=(const basic_int<X>& rhs)
    {
        *this = *this % rhs;

        return *this;
    }

    template <class X>
    basic_int<X>& basic_int<X>::operator&=(const basic_int<X>& rhs)
    {
        *this = *this & rhs;

        return *this;
    }

    template <class X>
    basic_int<X>& basic_int<X>::operator|=(const basic_int<X>& rhs)
    {
        *this = *this | rhs;

        return *this;
    }

    template <class X>
    basic_int<X>& basic_int<X>::operator^=(const basic_int<X>& rhs)
    {
        *this = *this ^ rhs;

        return *this;
    }

    template <class X>
    basic_int<X>& basic_int<X>::operator*=(const basic_int<X>& rhs)
    {
        *this = *this * rhs;

        return *this;
    }

    template <class X>
    basic_int<X>& basic_int<X>::operator/=(const basic_int<X>& rhs)
    {
        *this = *this / rhs;

        return *this;
    }

    template <class X>
    ostream& operator<<(ostream& stream, const basic_int<X>& huge)
    {
        stringstream ss;

        ss.flags(std::ios::hex | std::ios::uppercase);
        ss.fill('0');

        const std::vector<X>& buf = huge.m_Polynomial;
        for (const auto& x : buf)
        {
            ss.width(2);
            ss << static_cast<uint16_t>(x);
        }

        if (huge.m_Negative)
            stream << "-";
        stream << ss.str();
        return stream;
    }

    template <class T>
    short compare(const basic_int<T>& lhs, const basic_int<T>& rhs)
    {
        const auto& a = lhs.m_Polynomial;
        const auto& b = rhs.m_Polynomial;

        const short cmp = Cry_compare(&a[0], &a[0] + a.size(), &b[0], &b[0] + b.size());

        if (lhs.m_Negative && rhs.m_Negative)
        {
            if (cmp == -1)
            {
                return 1;
            }
            if (cmp == 1)
            {
                return -1;
            }

            return 0;
        }

        if (!lhs.m_Negative && !rhs.m_Negative)
        {
            return cmp;
        }

        return lhs.m_Negative ? -1 : 1;
    }

    template <class T>
    const basic_int<T> basic_int<T>::operator<<(int nbits) const
    {
        std::vector<T> out(m_Polynomial);

        Cry_lshift(&out[0], &out[0] + out.size(), nbits);

        return basic_int<T>(out);
    }

    template <class T>
    basic_int<T>& basic_int<T>::operator<<=(int nbits)
    {
        *this = *this << nbits;

        return *this;
    }

    template <class T>
    const basic_int<T> basic_int<T>::operator>>(int nbits) const
    {
        std::vector<T> out(m_Polynomial);

        Cry_rshift(&out[0], &out[0] + out.size(), nbits);

        return basic_int<T>(out);
    }

    template <class T>
    basic_int<T>& basic_int<T>::operator>>=(int nbits)
    {
        *this = *this >> nbits;

        return *this;
    }

    template <class X>
    bool operator!(const basic_int<X>& h)
    {
        const auto f = static_cast<bool>(h);

        return !f;
    }

    template <class T>
    void basic_int<T>::DivRem(basic_int<T>& q, basic_int<T>& r, const basic_int<T>& other) const
    {
        const bool is_zero = Cry_is_zero(&other.m_Polynomial[0], &other.m_Polynomial[0] + other.m_Polynomial.size());
        if (is_zero)
        {
            throw std::invalid_argument("division by zero");
        }

        const short cmp = Cry_compare(&m_Polynomial[0], &m_Polynomial[0] + m_Polynomial.size(), &other.m_Polynomial[0], &other.m_Polynomial[0] + other.m_Polynomial.size());
        if (cmp == -1)
        {
            q = basic_int<T>();
            r = *this;
            return;
        }

        size_t l_size = this->m_Polynomial.size();

        std::vector<T> v_div(l_size);
        std::vector<T> v_rem(l_size);

        Cry_divide(&v_div[0] + v_div.size(), &v_rem[0] + v_rem.size(), &m_Polynomial[0], &m_Polynomial[0] + m_Polynomial.size(), &other.m_Polynomial[0], &other.m_Polynomial[0] + other.m_Polynomial.size());

        const basic_int<T> div(v_div, this->m_Negative ^ other.m_Negative);
        const basic_int<T> rem(v_rem, this->m_Negative ^ other.m_Negative);

        q = div;
        r = rem;
    }

    using bigint8_t  = basic_int<uint32_t>;
    using bigint16_t = basic_int<uint16_t>;
    using bigint32_t = basic_int<uint32_t>;
    using bigint64_t = basic_int<uint64_t>;
}

#endif
