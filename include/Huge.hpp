
#ifndef HUGE_HPP
#define HUGE_HPP

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <vector>

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

            HUGE_And(&(*std::end(out)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

            return Huge(out);
        }

        friend const Huge operator|(const Huge& lhs, const Huge& rhs) {
            auto max = std::max(lhs.m_Buffer.size(), rhs.m_Buffer.size());

            std::vector<T> out(max);

            HUGE_Or(&(*std::end(out)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

            return Huge(out);
        }

        friend const Huge operator^(const Huge& lhs, const Huge& rhs) {
            auto max = std::max(lhs.m_Buffer.size(), rhs.m_Buffer.size());

            std::vector<T> out(max);

            HUGE_Xor(&(*std::end(out)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

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
                short cmp = HUGE_Compare(&(*std::begin(lhsBuf)), &(*std::end(lhsBuf)), &(*std::begin(rhsBuf)), &(*std::end(rhsBuf)));

                if (cmp == -1) { // (|a| < |b|) ==> (|b| - |a|)
                    HUGE_Subtract(&(*std::end(out)), &(*std::begin(rhsBuf)), &(*std::end(rhsBuf)), &(*std::begin(lhsBuf)), &(*std::end(lhsBuf)));

                    return Huge(out, rhs.m_Negative);
                } else if (cmp == +1) {
                    // (|a| > |b|) ==> (|a| - |b|)
                    HUGE_Subtract(&(*std::end(out)), &(*std::begin(lhsBuf)), &(*std::end(lhsBuf)), &(*std::begin(rhsBuf)), &(*std::end(rhsBuf)));

                    return Huge(out, lhs.m_Negative);
                }
            } else { // если знаки аргументов одинаковы
                HUGE_Add(&(*std::end(out)), &(*std::begin(lhsBuf)), &(*std::end(lhsBuf)), &(*std::begin(rhsBuf)), &(*std::end(rhsBuf)));

                return Huge<T>(out, lhs.m_Negative & rhs.m_Negative);
            }

            return Huge();
        }

        friend const Huge operator-(const Huge& lhs, const Huge& rhs) {
            short cmp = HUGE_Compare(&(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

            // если знаки аргументов различны: (a)-(-b), (-a)-(b) ==> ?(a+b)
            if (lhs.m_Negative ^ rhs.m_Negative) {
                Huge temp((cmp == -1) ? rhs : lhs);

                HUGE_Add(&(*std::end(temp.m_Buffer)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

                temp.m_Negative = lhs.m_Negative;

                return temp;
            }

            // если знаки аргументов одинаковы:
            if (cmp == -1) { // (|a| < |b|) ==> (|b| - |a|)
                Huge temp(rhs);

                HUGE_Subtract(&(*std::end(temp.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)));

                temp.m_Negative = (!lhs.m_Negative & !rhs.m_Negative);

                return temp;
            } else if (cmp == +1) {
                // (|a| > |b|) ==> (|a| - |b|)
                Huge temp(lhs);

                HUGE_Subtract(&(*std::end(temp.m_Buffer)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

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

            HUGE_Multiply(&(*std::begin(out)), &(*std::end(out)), &(*std::begin(lhsBuf)), &(*std::end(lhsBuf)), &(*std::begin(rhsBuf)), &(*std::end(rhsBuf)));

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
        bool flag = HUGE_IsZero(&(*std::begin(m_Buffer)), &(*std::end(m_Buffer)));

        return !flag;
    }

    template <class X>
    Huge<X>& Huge<X>::operator++() {
        HUGE_Increment(&(*std::begin(m_Buffer)), &(*std::end(m_Buffer)));

        return *this;
    }

    template <class X>
    Huge<X>& Huge<X>::operator--() {
        HUGE_Decrement(&(*std::begin(m_Buffer)), &(*std::end(m_Buffer)));

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

        HUGE_Inverse(&(*std::begin(temp.m_Buffer)), &(*std::end(temp.m_Buffer)));

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
        short cmp = HUGE_Compare(&(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

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
        std::vector<T> out(this->m_Buffer);

        HUGE_ShiftLeft(&(*std::begin(out)), &(*std::end(out)), nbits);

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

        HUGE_ShiftRight(&(*std::begin(out)), &(*std::end(out)), nbits);

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
    void Huge<T>::DivRem(Huge<T>& q, Huge<T>& r, const Huge<T>& other) const throw(std::invalid_argument) {
        bool isZero = HUGE_IsZero(&(*std::begin(other.m_Buffer)), &(*std::end(other.m_Buffer)));
        if (isZero) {
            throw std::invalid_argument("division by zero");
        }

        short cmp = HUGE_Compare(&(*std::begin(this->m_Buffer)), &(*std::end(this->m_Buffer)), &(*std::begin(other.m_Buffer)), &(*std::end(other.m_Buffer)));
        if (cmp == -1) {
            q = Huge<T>();
            r = *this;
            return;
        }

        size_t l_size = this->m_Buffer.size();

        std::vector<T> v_div(l_size);
        std::vector<T> v_rem(l_size);

        HUGE_DivRem(&(*std::begin(v_div)), &(*std::end(v_div)), &(*std::begin(v_rem)), &(*std::end(v_rem)), &(*std::begin(this->m_Buffer)), &(*std::end(this->m_Buffer)), &(*std::begin(other.m_Buffer)), &(*std::end(other.m_Buffer)));

        Huge<T> div(v_div, this->m_Negative ^ other.m_Negative);
        Huge<T> rem(v_rem, this->m_Negative ^ other.m_Negative);

        q = div;
        r = rem;
    }

    using bigint8_t = Huge<uint8_t >;
}

#endif