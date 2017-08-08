
#ifndef HUGE_HPP
#define HUGE_HPP

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <vector>

#include <cstdint>

#include <HugeCore.h>

using namespace std;

namespace cry {
    template <class T = uint8_t>
    class Huge {
      public:
        Huge() : m_Buffer(1), m_Negative(false) {}

        ~Huge() {}

        Huge(const Huge& other) : m_Buffer(other.m_Buffer), m_Negative(other.m_Negative) {}

        Huge(Huge&& other) { *this = std::move(other); }

        Huge(const std::initializer_list<T>& il, bool negative = false) : Huge(std::begin(il), std::end(il), negative) {}

        Huge(const std::vector<T>& iv, bool negative = false) : Huge(std::begin(iv), std::end(iv), negative) {}

        Huge(uint32_t x) : Huge({(T)((x & 0xff000000) >> 24), (T)((x & 0x00ff0000) >> 16), (T)((x & 0x0000ff00) >> 8), (T)(x & 0x000000ff)}) {}

        Huge(const std::string& hex):m_Negative(false)
        {
            // ------- skiping zeros -------
            auto it(hex.begin());
            while (*it == '0')
                ++it;

            std::string::const_reverse_iterator rit(hex.rbegin()), rend(it);

            size_t nchars = std::distance(rit, rend);
            size_t nbytes = nchars / 2;
            nbytes += nchars % 2;

            size_t nwords = nbytes / sizeof(T);
            if (nbytes >= sizeof(T))
                nwords += nbytes % sizeof(T);
            else
                nwords += 1;

            T word = 0;
            size_t cnt     = 0;

            m_Buffer.resize(nwords);
            auto ret(m_Buffer.rbegin());

            for (; rit != rend; ++rit)
            {
                uint8_t bt = *it;

                // transform hex character to the 4bit equivalent number, using the ascii table indexes
                if (bt >= '0' && bt <= '9')
                    bt = bt - '0';
                else if (bt >= 'a' && bt <= 'f')
                    bt = bt - 'a' + 10;
                else if (bt >= 'A' && bt <= 'F')
                    bt = bt - 'A' + 10;

                // shift 4 to make space for new digit, and add the 4 bits of the new digit
                // word = (word << 4) | (bt & 0xF);
                word = (bt << cnt * 4) | (word);

                ++cnt;

                if (cnt == sizeof(T) * 2)
                {
                    *ret++ = word;
                    word   = 0;
                    cnt    = 0;
                }
            }

            if (ret != m_Buffer.rend())
                *ret++ = word;
        }

        template <class InputIterator>
        Huge(InputIterator first, InputIterator last, bool negative = false)
            : m_Buffer(1), m_Negative(false) {
            for (; first != last && *first == 0x00; ++first)
                ;

            if (first != last) {
                m_Buffer = std::vector<T>(first, last);
                m_Negative = negative;
            }
        }

        Huge& operator=(const Huge& other);

        Huge& operator=(Huge&& other);

        explicit operator bool() const;

        const Huge operator<<(int) const;

        const Huge operator>>(int) const;

        Huge& operator<<=(int nbits);

        Huge& operator>>=(int nbits);

        Huge& operator++();

        Huge& operator--();

        Huge& operator+();

        const Huge operator++(int);

        const Huge operator--(int);

        const Huge operator-() const;

        const Huge operator~() const;

        Huge& operator+=(const Huge& rhs);

        Huge& operator-=(const Huge& rhs);

        Huge& operator%=(const Huge& rhs);

        Huge& operator&=(const Huge& rhs);

        Huge& operator|=(const Huge& rhs);

        Huge& operator^=(const Huge& rhs);

        Huge& operator*=(const Huge& rhs);

        Huge& operator/=(const Huge& rhs);

        template <class X>
        friend ostream& operator<<(ostream& stream, const Huge<X>& huge);

        friend bool operator&&(const Huge& lhs, const Huge& rhs) {
            bool f1 = (bool)lhs;
            bool f2 = (bool)rhs;

            return (f1 && f2);
        }

        friend bool operator||(const Huge& lhs, const Huge& rhs) {
            bool f1 = (bool)lhs;
            bool f2 = (bool)rhs;

            return (f1 || f2);
        }

        friend bool operator<(const Huge& lhs, const Huge& rhs) {
            short cmp = compare(lhs, rhs);

            return (cmp == -1);
        }

        friend bool operator>(const Huge& lhs, const Huge& rhs) { return rhs < lhs; }

        friend bool operator<=(const Huge& lhs, const Huge& rhs) { return !(lhs > rhs); }

        friend bool operator>=(const Huge& lhs, const Huge& rhs) { return !(lhs < rhs); }

        friend bool operator==(const Huge& lhs, const Huge& rhs) {
            short cmp = compare(lhs, rhs);

            return (cmp == 0);
        }

        friend bool operator!=(const Huge& lhs, const Huge& rhs) { return !(lhs == rhs); }

        friend const Huge operator&(const Huge& lhs, const Huge& rhs) {
            auto max = std::max(lhs.m_Buffer.size(), rhs.m_Buffer.size());

            std::vector<T> out(max);

            HUGE_And(&out[0] + out.size(), &lhs.m_Buffer[0], &lhs.m_Buffer[0] + lhs.m_Buffer.size(), &rhs.m_Buffer[0], &rhs.m_Buffer[0] + rhs.m_Buffer.size());

            return Huge(out);
        }

        friend const Huge operator|(const Huge& lhs, const Huge& rhs) {
            auto max = std::max(lhs.m_Buffer.size(), rhs.m_Buffer.size());

            std::vector<T> out(max);

            HUGE_Or(&out[0] + out.size(), &lhs.m_Buffer[0], &lhs.m_Buffer[0] + lhs.m_Buffer.size(), &rhs.m_Buffer[0], &rhs.m_Buffer[0] + rhs.m_Buffer.size());

            return Huge(out);
        }

        friend const Huge operator^(const Huge& lhs, const Huge& rhs) {
            auto max = std::max(lhs.m_Buffer.size(), rhs.m_Buffer.size());

            std::vector<T> out(max);

            HUGE_Xor(&out[0] + out.size(), &lhs.m_Buffer[0], &lhs.m_Buffer[0] + lhs.m_Buffer.size(), &rhs.m_Buffer[0], &rhs.m_Buffer[0] + rhs.m_Buffer.size());

            return Huge(out);
        }

        friend const Huge
        operator+(const Huge& lhs, const Huge& rhs) {
            const auto& lhsBuf = lhs.m_Buffer;
            const auto& rhsBuf = rhs.m_Buffer;

            size_t lsize = lhsBuf.size();
            size_t rsize = rhsBuf.size();

            std::vector<T> out(std::max(lsize, rsize));

            // если знаки аргументов различны: (a)+(-b), (-a)+(b) ==> ?(a-b)
            if (lhs.m_Negative ^ rhs.m_Negative) {
                short cmp = HUGE_Compare(&lhsBuf[0], &lhsBuf[0] + lsize, &rhsBuf[0], &rhsBuf[0] + rsize);

                if (cmp == -1) { // (|a| < |b|) ==> (|b| - |a|)
                    HUGE_Subtract(&out[0] + out.size(), &rhsBuf[0], &rhsBuf[0] + rsize, &lhsBuf[0], &lhsBuf[0] + lsize);

                    return Huge(out, rhs.m_Negative);
                } else if (cmp == +1) {
                    // (|a| > |b|) ==> (|a| - |b|)
                    HUGE_Subtract(&out[0] + out.size(), &lhsBuf[0], &lhsBuf[0] + lsize, &rhsBuf[0], &rhsBuf[0] + rsize);

                    return Huge(out, lhs.m_Negative);
                }
            } else { // если знаки аргументов одинаковы
                HUGE_Add(&out[0] + out.size(), &lhsBuf[0], &lhsBuf[0] + lsize, &rhsBuf[0], &rhsBuf[0] + rsize);

                return Huge<T>(out, lhs.m_Negative & rhs.m_Negative);
            }

            return Huge();
        }

        friend const Huge operator-(const Huge& lhs, const Huge& rhs) {
            short cmp = HUGE_Compare(&lhs.m_Buffer[0], &lhs.m_Buffer[0] + lhs.m_Buffer.size(), &rhs.m_Buffer[0], &rhs.m_Buffer[0] + rhs.m_Buffer.size());

            // если знаки аргументов различны: (a)-(-b), (-a)-(b) ==> ?(a+b)
            if (lhs.m_Negative ^ rhs.m_Negative) {
                Huge temp((cmp == -1) ? rhs : lhs);

                HUGE_Add(&temp.m_Buffer[0] + temp.m_Buffer.size(), &lhs.m_Buffer[0], &lhs.m_Buffer[0] + lhs.m_Buffer.size(), &rhs.m_Buffer[0], &rhs.m_Buffer[0] + rhs.m_Buffer.size());

                temp.m_Negative = lhs.m_Negative;

                return temp;
            }

            // если знаки аргументов одинаковы:
            if (cmp == -1) { // (|a| < |b|) ==> (|b| - |a|)
                Huge temp(rhs);

                HUGE_Subtract(&temp.m_Buffer[0] + temp.m_Buffer.size(), &rhs.m_Buffer[0], &rhs.m_Buffer[0] + rhs.m_Buffer.size(), &lhs.m_Buffer[0], &lhs.m_Buffer[0] + lhs.m_Buffer.size());

                temp.m_Negative = (!lhs.m_Negative & !rhs.m_Negative);

                return temp;
            } else if (cmp == +1) {
                // (|a| > |b|) ==> (|a| - |b|)
                Huge temp(lhs);

                HUGE_Subtract(&temp.m_Buffer[0] + temp.m_Buffer.size(), &lhs.m_Buffer[0], &lhs.m_Buffer[0] + lhs.m_Buffer.size(), &rhs.m_Buffer[0], &rhs.m_Buffer[0] + rhs.m_Buffer.size());

                temp.m_Negative = (lhs.m_Negative & rhs.m_Negative);

                return temp;
            }

            return Huge();
        }

        friend const Huge operator*(const Huge& lhs, const Huge& rhs) {
            const std::vector<byte>& lhsBuf = lhs.m_Buffer;
            const std::vector<byte>& rhsBuf = rhs.m_Buffer;

            size_t l_size = lhsBuf.size();
            size_t r_size = rhsBuf.size();

            std::vector<T> out(l_size + r_size);

            HUGE_Multiply(&out[0], &out[0] + out.size(), &lhsBuf[0], &lhsBuf[0] + l_size, &rhsBuf[0], &rhsBuf[0] + r_size);

            return Huge(out, lhs.m_Negative ^ rhs.m_Negative);
        }

        friend const Huge operator/(const Huge& lhs, const Huge& rhs) {
            Huge div;
            Huge rem;

            lhs.DivRem(div, rem, rhs);

            return div;
        }

        friend const Huge operator%(const Huge& lhs, const Huge& rhs) {
            Huge div;
            Huge rem;

            lhs.DivRem(div, rem, rhs);

            return rem;
        }

        void DivRem(Huge& q, Huge& r, const Huge& other) const throw(std::invalid_argument);

      protected:
        void __swap(Huge& other) throw() {
            m_Buffer.swap(other.m_Buffer);

            std::swap(m_Negative, other.m_Negative);
        }

        template <class X>
        friend short compare(const Huge<X>& lhs, const Huge<X>& rhs);

      private:
        std::vector<T> m_Buffer;
        bool m_Negative;
    };

    // =================================================================================

    template <class X>
    Huge<X>& Huge<X>::operator=(const Huge<X>& other) {
        if (this != &other) {
            Huge temp(other);

            this->__swap(temp);
        }

        return *this;
    }

    template <class X>
    Huge<X>& Huge<X>::operator=(Huge<X>&& other) {
        if (this != &other) {
            m_Buffer = std::move(other.m_Buffer);
            m_Negative = other.m_Negative;
        }

        return *this;
    }

    template <class X>
    Huge<X>::operator bool() const {
        bool flag = HUGE_IsZero(&m_Buffer[0], (&m_Buffer[0] + m_Buffer.size()));

        return !flag;
    }

    template <class X>
    Huge<X>& Huge<X>::operator++() {
        HUGE_Increment(&m_Buffer[0], &m_Buffer[0] + m_Buffer.size());

        return *this;
    }

    template <class X>
    Huge<X>& Huge<X>::operator--() {
        HUGE_Decrement(&m_Buffer[0], &m_Buffer[0] + m_Buffer.size());

        return *this;
    }

    template <class X>
    const Huge<X> Huge<X>::operator++(int) {
        Huge temp(*this);

        ++(*this);

        return temp;
    }

    template <class X>
    const Huge<X> Huge<X>::operator--(int) {
        Huge temp(*this);

        --(*this);

        return temp;
    }

    template <class X>
    Huge<X>& Huge<X>::operator+=(const Huge<X>& rhs) {
        *this = *this + rhs;

        return *this;
    }

    template <class X>
    Huge<X>& Huge<X>::operator-=(const Huge<X>& rhs) {
        *this = *this - rhs;

        return *this;
    }

    template <class X>
    Huge<X>& Huge<X>::operator+() {
        return (*this);
    }

    template <class X>
    const Huge<X> Huge<X>::operator-() const {
        return Huge(m_Buffer, !m_Negative);
    }

    template <class X>
    const Huge<X> Huge<X>::operator~() const {
        Huge temp(*this);

        HUGE_Inverse(&temp.m_Buffer[0], &temp.m_Buffer[0] + temp.m_Buffer.size());

        return temp;
    }

    template <class X>
    Huge<X>& Huge<X>::operator%=(const Huge<X>& rhs) {
        *this = *this % rhs;

        return *this;
    }

    template <class X>
    Huge<X>& Huge<X>::operator&=(const Huge<X>& rhs) {
        *this = *this & rhs;

        return *this;
    }

    template <class X>
    Huge<X>& Huge<X>::operator|=(const Huge<X>& rhs) {
        *this = *this | rhs;

        return *this;
    }

    template <class X>
    Huge<X>& Huge<X>::operator^=(const Huge<X>& rhs) {
        *this = *this ^ rhs;

        return *this;
    }

    template <class X>
    Huge<X>& Huge<X>::operator*=(const Huge<X>& rhs) {
        *this = *this * rhs;

        return *this;
    }

    template <class X>
    Huge<X>& Huge<X>::operator/=(const Huge<X>& rhs) {
        *this = *this / rhs;

        return *this;
    }

    template <class X>
    ostream& operator<<(ostream& stream, const Huge<X>& huge) {
        stringstream ss;

        ss.flags(std::ios::hex | std::ios::uppercase);
        ss.fill('0');

        const std::vector<X>& buf = huge.m_Buffer;
        for (const auto& x : buf) {
            ss.width(2);
            ss << (uint16_t)x;
        }

        if (huge.m_Negative)
            stream << "-";
        stream << ss.str();
        return stream;
    }

    template <class T>
    short compare(const Huge<T>& lhs, const Huge<T>& rhs) {
        short cmp = HUGE_Compare(&lhs.m_Buffer[0], &lhs.m_Buffer[0] + lhs.m_Buffer.size(), &rhs.m_Buffer[0], &rhs.m_Buffer[0] + rhs.m_Buffer.size());

        if (lhs.m_Negative && rhs.m_Negative) {
            if (cmp == -1) {
                return 1;
            } else if (cmp == 1) {
                return -1;
            }

            return 0;
        } else if (!lhs.m_Negative && !rhs.m_Negative) {
            return cmp;
        } else {
            return lhs.m_Negative ? -1 : 1;
        }
    }

    template <class T>
    const Huge<T> Huge<T>::operator<<(int nbits) const {
        std::vector<T> out(m_Buffer);

        HUGE_ShiftLeft(&out[0], &out[0] + out.size(), nbits);

        return Huge<T>(out);
    }

    template <class T>
    Huge<T>& Huge<T>::operator<<=(int nbits) {
        *this = *this << nbits;

        return *this;
    }

    template <class T>
    const Huge<T> Huge<T>::operator>>(int nbits) const {
        std::vector<T> out(this->m_Buffer);

        HUGE_ShiftRight(&out[0], &out[0] + out.size(), nbits);

        return Huge<T>(out);
    }

    template <class T>
    Huge<T>& Huge<T>::operator>>=(int nbits) {
        *this = *this >> nbits;

        return *this;
    }

    template <class X>
    bool operator!(const Huge<X>& h) {
        bool f = (bool)h;

        return !f;
    }

    template <class T>
    void Huge<T>::DivRem(Huge<T>& q, Huge<T>& r, const Huge<T>& other) const throw(std::invalid_argument){
        bool isZero = HUGE_IsZero(&other.m_Buffer[0], &other.m_Buffer[0] + other.m_Buffer.size());
        if (isZero) {
            throw std::invalid_argument("division by zero");
        }

        short cmp = HUGE_Compare(&m_Buffer[0], &m_Buffer[0] + m_Buffer.size(), &other.m_Buffer[0], &other.m_Buffer[0] + other.m_Buffer.size());
        if (cmp == -1) {
            q = Huge<T>();
            r = *this;
            return;
        }

        size_t l_size = this->m_Buffer.size();

        std::vector<T> v_div(l_size);
        std::vector<T> v_rem(l_size);

        HUGE_DivRem(&v_div[0], &v_div[0] + v_div.size(), &v_rem[0], &v_rem[0] + v_rem.size(), &m_Buffer[0], &m_Buffer[0] + m_Buffer.size(), &other.m_Buffer[0], &other.m_Buffer[0] + other.m_Buffer.size());

        Huge<T> div(v_div, this->m_Negative ^ other.m_Negative);
        Huge<T> rem(v_rem, this->m_Negative ^ other.m_Negative);

        q = div;
        r = rem;
    }

    using bigint8_t = Huge<uint8_t >;
}

#endif
