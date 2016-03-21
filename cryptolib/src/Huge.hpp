
#ifndef HUGE_HPP
#define HUGE_HPP

#include <vector>
#include <algorithm>
#include <iterator>
#include <initializer_list>
#include <iostream>

#include "HugeCore.h"

using namespace std;

template <class T>
class Huge
{
  public:
    Huge() : m_Buffer(1), m_Negative(false)
    {
    }

    ~Huge()
    {
    }

    Huge(const Huge& other) : m_Buffer(other.m_Buffer), m_Negative(other.m_Negative)
    {
    }

    Huge(const std::initializer_list<T>& il, bool negative = false) : m_Buffer(il), m_Negative(negative)
    {
    }

    Huge(const std::vector<T>& iv, bool negative = false) : m_Buffer(iv), m_Negative(negative)
    {
    }

    Huge& operator=(const Huge& other);

    operator bool() const;

    Huge operator<<(int);

    Huge operator>>(int);

    Huge& operator<<=(int nbits);

    Huge& operator>>=(int nbits);

    Huge& operator++();

    Huge& operator--();

    Huge operator++(int);

    Huge operator--(int);

    Huge& operator+=(const Huge& rhs);

    Huge& operator-=(const Huge& rhs);

    Huge& operator+();

    Huge operator-();

    Huge operator~();

    Huge operator%=(const Huge& rhs);

    Huge operator&(const Huge& rhs);

    Huge& operator&=(const Huge& rhs);

    Huge operator|(const Huge& rhs);

    Huge& operator|=(const Huge& rhs);

    Huge operator^(const Huge& rhs);

    Huge& operator^=(const Huge& rhs);

    Huge& operator*=(const Huge& rhs);

    Huge& operator/=(const Huge& rhs);

    template <class X>
    friend ostream& operator<<(ostream& stream, const Huge<X>& huge);

    template <class X>
    friend Huge<X> operator+(const Huge<X>&, const Huge<X>&);

    template <class X>
    friend Huge<X> operator-(const Huge<X>&, const Huge<X>&);

    template <class X>
    friend Huge<X> operator*(const Huge<X>&, const Huge<X>&);

    template <class X>
    friend Huge<X> operator/(const Huge<X>&, const Huge<X>&);

    template <class X>
    friend Huge<X> operator%(const Huge<X>&, const Huge<X>&);

    Huge Gcd(const Huge& other);

  protected:
    void __swap(Huge& other) throw()
    {
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
Huge<X>& Huge<X>::operator=(const Huge<X>& other)
{
    if (this != &other)
    {
        Huge temp(other);

        this->__swap(temp);
    }

    return *this;
}

template <class X>
Huge<X>::operator bool() const
{
    bool flag = HUGE_IsZero(&(*std::begin(m_Buffer)), &(*std::end(m_Buffer)));

    return !flag;
}

template <class X>
Huge<X>& Huge<X>::operator++()
{
    HUGE_Increment(&(*std::begin(m_Buffer)), &(*std::end(m_Buffer)));

    return *this;
}

template <class X>
Huge<X>& Huge<X>::operator--()
{
    HUGE_Decrement(&(*std::begin(m_Buffer)), &(*std::end(m_Buffer)));

    return *this;
}

template <class X>
Huge<X> Huge<X>::operator++(int)
{
    Huge temp(*this);

    ++(*this);

    return temp;
}

template <class X>
Huge<X> Huge<X>::operator--(int)
{
    Huge temp(*this);

    --(*this);

    return temp;
}

template <class X>
Huge<X>& Huge<X>::operator+=(const Huge<X>& rhs)
{
    *this = *this + rhs;

    return *this;
}

template <class X>
Huge<X>& Huge<X>::operator-=(const Huge<X>& rhs)
{
    *this = *this - rhs;

    return *this;
}

template <class X>
Huge<X>& Huge<X>::operator+()
{
    return (*this);
}

template <class X>
Huge<X> Huge<X>::operator-()
{
    return Huge(m_Buffer, !m_Negative);
}

template <class X>
Huge<X> Huge<X>::operator~()
{
    Huge temp(*this);

    HUGE_Inverse(&(*std::begin(temp.m_Buffer)), &(*std::end(temp.m_Buffer)));

    return temp;
}

template <class X>
Huge<X> Huge<X>::operator%=(const Huge<X>& rhs)
{
    *this = *this % rhs;

    return *this;
}

template <class X>
Huge<X> Huge<X>::operator&(const Huge<X>& rhs)
{
    auto max = std::max(m_Buffer.size(), rhs.m_Buffer.size());

    std::vector<X> temp((std::vector<X>(max)));

    HUGE_And(&(*std::end(temp)), &(*std::begin(m_Buffer)), &(*std::end(m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

    return Huge(temp);
}

template <class X>
Huge<X>& Huge<X>::operator&=(const Huge<X>& rhs)
{
    *this = *this & rhs;

    return *this;
}

template <class X>
Huge<X> Huge<X>::operator|(const Huge<X>& rhs)
{
    auto max = std::max(m_Buffer.size(), rhs.m_Buffer.size());

    std::vector<X> temp((std::vector<X>(max)));

    HUGE_Or(&(*std::end(temp)), &(*std::begin(m_Buffer)), &(*std::end(m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

    return Huge(temp);
}

template <class X>
Huge<X>& Huge<X>::operator|=(const Huge<X>& rhs)
{
    *this = *this | rhs;

    return *this;
}

template <class X>
Huge<X> Huge<X>::operator^(const Huge<X>& rhs)
{
    auto max = std::max(m_Buffer.size(), rhs.m_Buffer.size());

    std::vector<X> temp((std::vector<X>(max)));

    HUGE_Xor(&(*std::end(temp)), &(*std::begin(m_Buffer)), &(*std::end(m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

    return Huge(temp);
}

template <class X>
Huge<X>& Huge<X>::operator^=(const Huge<X>& rhs)
{
    *this = *this ^ rhs;

    return *this;
}

template <class X>
Huge<X>& Huge<X>::operator*=(const Huge<X>& rhs)
{
    *this = *this * rhs;

    return *this;
}

template <class X>
Huge<X>& Huge<X>::operator/=(const Huge<X>& rhs)
{
    *this = *this / rhs;

    return *this;
}

template <class X>
ostream& operator<<(ostream& stream, const Huge<X>& huge)
{
    stringstream ss;

    ss.flags(std::ios::hex | std::ios::uppercase);
    ss.width(2);
    ss.fill('0');

    const std::vector<byte>& buf = huge.m_Buffer;
    for (const auto& x : buf)
    {
        ss << (uint16_t)x;
    }

    stream << ss.str();
    return stream;
}

template <class T>
short compare(const Huge<T>& lhs, const Huge<T>& rhs)
{
    short cmp = HUGE_Compare(&(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

    if (lhs.m_Negative && rhs.m_Negative)
    {
        if (cmp == -1)
        {
            return 1;
        }
        else if (cmp == 1)
        {
            return -1;
        }

        return 0;
    }
    else if (!lhs.m_Negative && !rhs.m_Negative)
    {
        return cmp;
    }
    else
    {
        return lhs.m_Negative ? -1 : 1;
    }
}

template <class T>
Huge<T> Huge<T>::operator<<(int nbits)
{
    Huge<T> temp(*this);

    HUGE_ShiftLeft(&(*std::begin(temp.m_Buffer)), &(*std::end(temp.m_Buffer)), nbits);

    return temp;
}

template <class T>
Huge<T>& Huge<T>::operator<<=(int nbits)
{
    HUGE_ShiftLeft(&(*std::begin(m_Buffer)), &(*std::end(m_Buffer)), nbits);

    return *this;
}

template <class T>
Huge<T> Huge<T>::operator>>(int nbits)
{
    Huge<T> temp(*this);

    HUGE_ShiftRight(&(*std::begin(temp.m_Buffer)), &(*std::end(temp.m_Buffer)), nbits);

    return temp;
}

template <class T>
Huge<T>& Huge<T>::operator>>=(int nbits)
{
    HUGE_ShiftRight(&(*std::begin(m_Buffer)), &(*std::end(m_Buffer)), nbits);

    return *this;
}

template <class X>
bool operator&&(const Huge<X>& lhs, const Huge<X>& rhs)
{
    bool f1 = lhs;
    bool f2 = rhs;

    return (f1 && f2);
}

template <class X>
bool operator||(const Huge<X>& lhs, const Huge<X>& rhs)
{
    bool f1 = lhs;
    bool f2 = rhs;

    return (f1 || f2);
}

template <class X>
bool operator!(const Huge<X>& h)
{
    bool f = h;

    return !f;
}

template <class X>
bool operator<(const Huge<X>& lhs, const Huge<X>& rhs)
{
    short cmp = compare(lhs, rhs);

    return (cmp == -1);
}

template <class T>
bool operator>(const Huge<T>& lhs, const Huge<T>& rhs)
{
    short cmp = compare(lhs, rhs);

    return (cmp == 1);
}

template <class T>
bool operator==(const Huge<T>& lhs, const Huge<T>& rhs)
{
    short cmp = compare(lhs, rhs);

    return (cmp == 0);
}

template <class T>
bool operator<=(const Huge<T>& lhs, const Huge<T>& rhs)
{
    short cmp = compare(lhs, rhs);

    return ((cmp == -1) || (cmp == 0));
}

template <class T>
bool operator>=(const Huge<T>& lhs, const Huge<T>& rhs)
{
    short cmp = compare(lhs, rhs);

    return ((cmp == 1) || (cmp == 0));
}

template <class T>
bool operator!=(const Huge<T>& lhs, const Huge<T>& rhs)
{
    short cmp = compare(lhs, rhs);

    return (cmp != 0);
}

template <class X>
Huge<X> operator+(const Huge<X>& lhs, const Huge<X>& rhs)
{
    const auto& lhsBuf = lhs.m_Buffer;
    const auto& rhsBuf = rhs.m_Buffer;

    // если знаки аргументов различны: (a)+(-b), (-a)+(b) ==> ?(a-b)
    if (lhs.m_Negative ^ rhs.m_Negative)
    {
        short cmp = HUGE_Compare(&(*std::begin(lhsBuf)), &(*std::end(lhsBuf)), &(*std::begin(rhsBuf)), &(*std::end(rhsBuf)));

        if (cmp == -1)
        { // (|a| < |b|) ==> (|b| - |a|)
            Huge<X> temp(rhs);

            HUGE_Subtract(&(*std::end(temp.m_Buffer)), &(*std::begin(rhsBuf)), &(*std::end(rhsBuf)), &(*std::begin(lhsBuf)), &(*std::end(lhsBuf)));

            return temp;
        }
        else if (cmp == +1)
        {
            // (|a| > |b|) ==> (|a| - |b|)
            Huge<X> temp(lhs);

            HUGE_Subtract(&(*std::end(temp.m_Buffer)), &(*std::begin(lhsBuf)), &(*std::end(lhsBuf)), &(*std::begin(rhsBuf)), &(*std::end(rhsBuf)));

            return temp;
        }
    }
    else
    { // если знаки аргументов одинаковы
        Huge<X> temp(std::max(lhs, rhs));

        HUGE_Add(&(*std::end(temp.m_Buffer)), &(*std::begin(lhsBuf)), &(*std::end(lhsBuf)), &(*std::begin(rhsBuf)), &(*std::end(rhsBuf)));

        temp.m_Negative = lhs.m_Negative & rhs.m_Negative;

        return temp;
    }

    return Huge<X>();
}

template <class X>
Huge<X> operator-(const Huge<X>& lhs, const Huge<X>& rhs)
{
    short cmp = HUGE_Compare(&(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

    // если знаки аргументов различны: (a)-(-b), (-a)-(b) ==> ?(a+b)
    if (lhs.m_Negative ^ rhs.m_Negative)
    {
        Huge<X> temp((cmp == -1) ? rhs : lhs);

        HUGE_Add(&(*std::end(temp.m_Buffer)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

        temp.m_Negative = lhs.m_Negative;

        return temp;
    }

    // если знаки аргументов одинаковы:
    if (cmp == -1)
    { // (|a| < |b|) ==> (|b| - |a|)
        Huge<X> temp(rhs);

        HUGE_Subtract(&(*std::end(temp.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)));

        temp.m_Negative = (!lhs.m_Negative & !rhs.m_Negative);

        return temp;
    }
    else if (cmp == +1)
    {
        // (|a| > |b|) ==> (|a| - |b|)
        Huge<X> temp(lhs);

        HUGE_Subtract(&(*std::end(temp.m_Buffer)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

        temp.m_Negative = (lhs.m_Negative & rhs.m_Negative);

        return temp;
    }

    return Huge<X>();
}

template <class X>
Huge<X> operator*(const Huge<X>& lhs, const Huge<X>& rhs)
{
    size_t l_size = lhs.m_Buffer.size();
    size_t r_size = rhs.m_Buffer.size();

    const std::vector<byte>& lhsBuf = lhs.m_Buffer;
    const std::vector<byte>& rhsBuf = rhs.m_Buffer;

    Huge<X> tmp((std::vector<byte>(l_size + r_size)));

    std::vector<byte>& buf = tmp.m_Buffer;
    HUGE_Multiply(&(*std::begin(buf)), &(*std::end(buf)), &(*std::begin(lhsBuf)), &(*std::end(lhsBuf)), &(*std::begin(rhsBuf)), &(*std::end(rhsBuf)));
    tmp.m_Negative = lhs.m_Negative ^ rhs.m_Negative;

    return tmp;
}

template <class X>
Huge<X> operator/(const Huge<X>& lhs, const Huge<X>& rhs)
{
    size_t l_size = lhs.m_Buffer.size();

    const std::vector<byte>& lhsBuf = lhs.m_Buffer;
    const std::vector<byte>& rhsBuf = rhs.m_Buffer;

    Huge<X> div((std::vector<byte>(l_size)));
    Huge<X> rem((std::vector<byte>(l_size)));

    std::vector<byte>& divBuf = div.m_Buffer;
    std::vector<byte>& remBuf = rem.m_Buffer;

    div.m_Negative = lhs.m_Negative ^ rhs.m_Negative;

    HUGE_DivRem(&(*std::begin(divBuf)), &(*std::end(divBuf)), &(*std::begin(remBuf)), &(*std::end(remBuf)), &(*std::begin(lhsBuf)), &(*std::end(lhsBuf)), &(*std::begin(rhsBuf)), &(*std::end(rhsBuf)));

    return div;
}

template <class X>
Huge<X> operator%(const Huge<X>& lhs, const Huge<X>& rhs)
{
    size_t l_size = lhs.m_Buffer.size();

    const std::vector<byte>& lhsBuf = lhs.m_Buffer;
    const std::vector<byte>& rhsBuf = rhs.m_Buffer;

    Huge<X> div((std::vector<byte>(l_size)));
    Huge<X> rem((std::vector<byte>(l_size)));

    std::vector<byte>& divBuf = div.m_Buffer;
    std::vector<byte>& remBuf = rem.m_Buffer;

    rem.m_Negative = lhs.m_Negative ^ rhs.m_Negative;

    HUGE_DivRem(&(*std::begin(divBuf)), &(*std::end(divBuf)), &(*std::begin(remBuf)), &(*std::end(remBuf)), &(*std::begin(lhsBuf)), &(*std::end(lhsBuf)), &(*std::begin(rhsBuf)), &(*std::end(rhsBuf)));

    return rem;
}

template <class T>
Huge<T> Huge<T>::Gcd(const Huge<T>& other)
{
    auto pair = std::minmax(*this, other);

    Huge<byte> r1(pair.second);
    Huge<byte> r2(pair.first);

    while (r2 != 0)
    {
        Huge<byte> rem = r1 % r2;
        r1             = r2;
        r2             = rem;
    }

    return r1;
}

#endif
