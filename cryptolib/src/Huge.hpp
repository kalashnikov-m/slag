
#ifndef HUGE_HPP
#define HUGE_HPP

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <vector>

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

    Huge(Huge&& other)
    {
        *this = std::move(other);
    }

    Huge(const std::initializer_list<T>& il, bool negative = false) : Huge(std::begin(il), std::end(il), negative)
    {
    }

    Huge(const std::vector<T>& iv, bool negative = false) : Huge(std::begin(iv), std::end(iv), negative)
    {
    }

    template <class InputIterator>
    Huge(InputIterator first, InputIterator last, bool negative = false)
        : m_Buffer(1), m_Negative(false)
    {
        for (; first != last && *first == 0x00; ++first)
            ;

        if (first != last)
        {
            m_Buffer   = std::vector<T>(first, last);
            m_Negative = negative;
        }
    }

    Huge& operator=(const Huge& other);

    Huge& operator=(Huge&& other);

    operator bool() const;

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

    template <class X>
    friend const Huge<X> operator&(const Huge<X>& lhs, const Huge<X>& rhs);

    template <class X>
    friend const Huge<X> operator|(const Huge<X>& lhs, const Huge<X>& rhs);

    template <class X>
    friend const Huge<X> operator^(const Huge<X>& lhs, const Huge<X>& rhs);

    template <class X>
    friend const Huge<X> operator+(const Huge<X>&, const Huge<X>&);

    template <class X>
    friend const Huge<X> operator-(const Huge<X>&, const Huge<X>&);

    template <class X>
    friend const Huge<X> operator*(const Huge<X>&, const Huge<X>&);

    template <class X>
    friend const Huge<X> operator/(const Huge<X>&, const Huge<X>&);

    template <class X>
    friend const Huge<X> operator%(const Huge<X>&, const Huge<X>&);

    const Huge Gcd(const Huge& other) const;

    void DivRem(Huge& q, Huge& r, const Huge& other) const throw(std::invalid_argument);

    bool ModInverse(Huge<T>& inv, const Huge<T>& N) const;

    bool IsEven() const;

    bool IsOdd() const;

    const Huge Pow(const Huge& exponent) const;

    const Huge PowMod(const Huge& exponent, const Huge& mod) const;

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
Huge<X>& Huge<X>::operator=(Huge<X>&& other)
{
    if (this != &other)
    {
        m_Buffer   = std::move(other.m_Buffer);
        m_Negative = other.m_Negative;
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
const Huge<X> Huge<X>::operator++(int)
{
    Huge temp(*this);

    ++(*this);

    return temp;
}

template <class X>
const Huge<X> Huge<X>::operator--(int)
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
const Huge<X> Huge<X>::operator-() const
{
    return Huge(m_Buffer, !m_Negative);
}

template <class X>
const Huge<X> Huge<X>::operator~() const
{
    Huge temp(*this);

    HUGE_Inverse(&(*std::begin(temp.m_Buffer)), &(*std::end(temp.m_Buffer)));

    return temp;
}

template <class X>
Huge<X>& Huge<X>::operator%=(const Huge<X>& rhs)
{
    *this = *this % rhs;

    return *this;
}

template <class X>
Huge<X>& Huge<X>::operator&=(const Huge<X>& rhs)
{
    *this = *this & rhs;

    return *this;
}

template <class X>
Huge<X>& Huge<X>::operator|=(const Huge<X>& rhs)
{
    *this = *this | rhs;

    return *this;
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
    ss.fill('0');

    const std::vector<byte>& buf = huge.m_Buffer;
    for (const auto& x : buf)
    {
        ss.width(2);
        ss << (uint16_t)x;
    }

    if (huge.m_Negative)
        stream << "-";
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
const Huge<T> Huge<T>::operator<<(int nbits) const
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
const Huge<T> Huge<T>::operator>>(int nbits) const
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
const Huge<X> operator&(const Huge<X>& lhs, const Huge<X>& rhs)
{
    auto max = std::max(lhs.m_Buffer.size(), rhs.m_Buffer.size());

    std::vector<X> temp((std::vector<X>(max)));

    HUGE_And(&(*std::end(temp)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

    return Huge<X>(temp);
}

template <class X>
const Huge<X> operator|(const Huge<X>& lhs, const Huge<X>& rhs)
{
    auto max = std::max(lhs.m_Buffer.size(), rhs.m_Buffer.size());

    std::vector<X> temp((std::vector<X>(max)));

    HUGE_Or(&(*std::end(temp)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

    return Huge<X>(temp);
}

template <class X>
const Huge<X> operator^(const Huge<X>& lhs, const Huge<X>& rhs)
{
    auto max = std::max(lhs.m_Buffer.size(), rhs.m_Buffer.size());

    std::vector<X> temp((std::vector<X>(max)));

    HUGE_Xor(&(*std::end(temp)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

    return Huge<X>(temp);
}

template <class X>
const Huge<X> operator+(const Huge<X>& lhs, const Huge<X>& rhs)
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
const Huge<X> operator-(const Huge<X>& lhs, const Huge<X>& rhs)
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
const Huge<X> operator*(const Huge<X>& lhs, const Huge<X>& rhs)
{
    const std::vector<byte>& lhsBuf = lhs.m_Buffer;
    const std::vector<byte>& rhsBuf = rhs.m_Buffer;

    size_t l_size = lhsBuf.size();
    size_t r_size = rhsBuf.size();

    std::vector<X> out(l_size + r_size);

    HUGE_Multiply(&(*std::begin(out)), &(*std::end(out)), &(*std::begin(lhsBuf)), &(*std::end(lhsBuf)), &(*std::begin(rhsBuf)), &(*std::end(rhsBuf)));

    return Huge<X>(out, lhs.m_Negative ^ rhs.m_Negative);
}

template <class X>
const Huge<X> operator/(const Huge<X>& lhs, const Huge<X>& rhs)
{
    Huge<X> div;
    Huge<X> rem;

    lhs.DivRem(div, rem, rhs);

    return div;
}

template <class X>
const Huge<X> operator%(const Huge<X>& lhs, const Huge<X>& rhs)
{
    Huge<X> div;
    Huge<X> rem;

    lhs.DivRem(div, rem, rhs);

    return rem;
}

template <class T>
const Huge<T> Huge<T>::Gcd(const Huge<T>& other) const
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

template <class T>
void Huge<T>::DivRem(Huge<T>& q, Huge<T>& r, const Huge<T>& other) const throw(std::invalid_argument)
{
    bool isZero = HUGE_IsZero(&(*std::begin(other.m_Buffer)), &(*std::end(other.m_Buffer)));
    if (isZero)
    {
        throw std::invalid_argument("division by zero");
    }

    short cmp = HUGE_Compare(&(*std::begin(this->m_Buffer)), &(*std::end(this->m_Buffer)), &(*std::begin(other.m_Buffer)), &(*std::end(other.m_Buffer)));
    if (cmp == -1)
    {
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

template <class T>
bool Huge<T>::ModInverse(Huge<T>& inv, const Huge<T>& N) const
{
    if (*this >= N)
        return false;

    Huge<T> r1(N);
    Huge<T> r2(*this);

    Huge<T> t1 = {0};
    Huge<T> t2 = {1};

    while (r2 > 0)
    {
        Huge<T> q;
        Huge<T> r;

        r1.DivRem(q, r, r2);
        r1 = r2;
        r2 = r;

        Huge<T> t = (t1 - q * t2);
        t1        = t2;
        t2        = t;
    }

    if (r1 != 1)
        return false;

    if (t1 < Huge<byte>({0}))
    {
        t1 += N;
    }

    inv = t1;

    return true;
}

template <class T>
bool Huge<T>::IsEven() const
{
    bool rv = HUGE_IsEven(&(*std::begin(this->m_Buffer)), &(*std::end(this->m_Buffer)));
    return rv;
}

template <class T>
bool Huge<T>::IsOdd() const
{
    bool rv = HUGE_IsOdd(&(*std::begin(this->m_Buffer)), &(*std::end(this->m_Buffer)));
    return rv;
}

template <class T>
const Huge<T> Huge<T>::Pow(const Huge<T>& exp) const
{
    Huge<T> y = {0x01};
    Huge<T> a = *this;
    Huge<T> e = exp;

    while (e > 0)
    {
        if (e.IsOdd())
            y *= a;

        a *= a;
        e >>= 1;
    }

    return y;
}

template <class T>
const Huge<T> Huge<T>::PowMod(const Huge<T>& exp, const Huge<T>& mod) const
{
    Huge<T> y = {0x01};
    Huge<T> a = *this;
    Huge<T> e = exp;

    while (e > 0)
    {
        if (e.IsOdd())
        {
            y *= a;
            y %= mod;
        }

        a *= a;
        a %= mod;

        e >>= 1;
    }

    return y;
}

#endif
