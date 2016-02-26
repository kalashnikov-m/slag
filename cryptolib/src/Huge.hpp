
#ifndef HUGE_HPP
#define HUGE_HPP

#include <vector>
#include <algorithm>
#include <iterator>
#include <initializer_list>

#include "HugeCore.h"

using namespace std;

typedef unsigned char byte;

template<class T>
class Huge
{
    public:

        Huge()
                : m_Negative(false)
        {
        }

        ~Huge()
        {
        }

        Huge(const Huge& other)
                : m_Buffer(other.m_Buffer), m_Negative(other.m_Negative)
        {
        }

        // HugeInt(HugeInt&& other);

        Huge(const std::initializer_list<T>& il, bool negative = false)
                : m_Buffer(il), m_Negative(negative)
        {
        }

        Huge(const std::vector<T>& iv, bool negative = false)
                : m_Buffer(iv), m_Negative(negative)
        {
        }

        Huge& operator = (const Huge& other)
        {
            if (this != &other)
            {
                Huge temp(other);

                this->__swap(temp);
            }

            return *this;
        }

        operator bool() const
        {
            bool flag = HUGE_IsZero(&(*std::begin(m_Buffer)), &(*std::end(m_Buffer)));

            return !flag;
        }

        Huge operator <<(int);

        Huge operator >>(int);

        Huge& operator <<= (int nbits);

        Huge& operator >>= (int nbits);

        Huge& operator ++()
        {
            HUGE_Increment(&(*std::begin(m_Buffer)), &(*std::end(m_Buffer)));

            return *this;
        }

        Huge& operator --()
        {
            HUGE_Decrement(&(*std::begin(m_Buffer)), &(*std::end(m_Buffer)));

            return *this;
        }

        Huge operator ++(int)
        {
            Huge temp(*this);

            ++(*this);

            return temp;
        }

        Huge operator --(int)
        {
            Huge temp(*this);

            --(*this);

            return temp;
        }

        Huge& operator += (const Huge& rhs)
        {
            *this = *this + rhs;

            return *this;
        }

        Huge& operator -= (const Huge& rhs)
        {
            *this = *this - rhs;

            return *this;
        }

        Huge& operator +()
        {
            return (*this);
        }

        Huge operator -()
        {
            return Huge(m_Buffer, !m_Negative);
        }

        Huge operator ~()
        {
            Huge temp(*this);

            HUGE_Inverse(&(*std::begin(temp.m_Buffer)), &(*std::end(temp.m_Buffer)));

            return temp;
        }

        Huge operator %= (const Huge& rhs)
        {
            *this = *this % rhs;

            return *this;
        }

        Huge operator &(const Huge& rhs)
        {
            auto max = std::max(m_Buffer.size(), rhs.m_Buffer.size());

            std::vector<T> temp((std::vector<T>(max)));

            HUGE_And(&(*std::end(temp)), &(*std::begin(m_Buffer)), &(*std::end(m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

            return Huge(temp);
        }

        Huge& operator &= (const Huge& rhs)
        {
            *this = *this & rhs;

            return *this;
        }

        Huge operator |(const Huge& rhs)
        {
            auto max = std::max(m_Buffer.size(), rhs.m_Buffer.size());

            std::vector<T> temp((std::vector<T>(max)));

            HUGE_Or(&(*std::end(temp)), &(*std::begin(m_Buffer)), &(*std::end(m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

            return Huge(temp);
        }

        Huge& operator |= (const Huge& rhs)
        {
            *this = *this | rhs;

            return *this;
        }

        Huge operator ^(const Huge& rhs)
        {
            auto max = std::max(m_Buffer.size(), rhs.m_Buffer.size());

            std::vector<T> temp((std::vector<T>(max)));

            HUGE_Xor(&(*std::end(temp)), &(*std::begin(m_Buffer)), &(*std::end(m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

            return Huge(temp);
        }

        Huge& operator ^= (const Huge& rhs)
        {
            *this = *this ^ rhs;

            return *this;
        }

        Huge& operator *= (const Huge& rhs)
        {
            *this = *this * rhs;

            return *this;
        }

        Huge& operator /= (const Huge& rhs)
        {
            *this = *this / rhs;

            return *this;
        }

        // Huge& operator=(Huge&& other);

        friend ostream& operator <<(ostream&, const Huge&)
        {    /* TODO */
        }

        template<class X>
        friend Huge<X> operator +(const Huge<X>&, const Huge<X>&);

        template<class X>
        friend Huge<X> operator -(const Huge<X>&, const Huge<X>&);

        template<class X>
        friend Huge<X> operator *(const Huge<X>&, const Huge<X>&);

        template<class X>
        friend Huge<X> operator /(const Huge<X>&, const Huge<X>&);

        template<class X>
        friend Huge<X> operator %(const Huge<X>&, const Huge<X>&);

    protected:
        void __swap(Huge& other) throw ()
        {
            m_Buffer.swap(other.m_Buffer);

            std::swap(m_Negative, other.m_Negative);
        }

        template<class X>
        friend short compare(const Huge<X>& lhs, const Huge<X>& rhs);

    private:

        std::vector<T> m_Buffer;
        bool           m_Negative;
};


template<class T>
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
    else if (!lhs.m_Negative &&!rhs.m_Negative)
    {
        return cmp;
    }
    else
    {
        return lhs.m_Negative ? -1 : 1;
    }
}

template<class T>
Huge<T> Huge<T>::operator <<(int nbits)
{
    Huge<T> temp(*this);

    HUGE_ShiftLeft(&(*std::begin(temp.m_Buffer)), &(*std::end(temp.m_Buffer)), nbits);

    return temp;
}

template<class T>
Huge<T>& Huge<T>::operator <<= (int nbits)
{
    HUGE_ShiftLeft(&(*std::begin(m_Buffer)), &(*std::end(m_Buffer)), nbits);

    return *this;
}

template<class T>
Huge<T> Huge<T>::operator >>(int nbits)
{
    Huge<T> temp(*this);

    HUGE_ShiftRight(&(*std::begin(temp.m_Buffer)), &(*std::end(temp.m_Buffer)), nbits);

    return temp;
}

template<class T>
Huge<T>& Huge<T>::operator >>= (int nbits)
{
    HUGE_ShiftRight(&(*std::begin(m_Buffer)), &(*std::end(m_Buffer)), nbits);

    return *this;
}

template<class X>
bool operator &&(const Huge<X>& lhs, const Huge<X>& rhs)
{
    bool f1 = lhs;
    bool f2 = rhs;

    return (f1 && f2);
}

template<class X>
bool operator ||(const Huge<X>& lhs, const Huge<X>& rhs)
{
    bool f1 = lhs;
    bool f2 = rhs;

    return (f1 || f2);
}

template<class X>
bool operator !(const Huge<X>& h)
{
    bool f = h;

    return !f;
}

template<class X>
bool operator <(const Huge<X>& lhs, const Huge<X>& rhs)
{
    short cmp = compare(lhs, rhs);

    return (cmp == -1);
}

template<class T>
bool operator >(const Huge<T>& lhs, const Huge<T>& rhs)
{
    short cmp = compare(lhs, rhs);

    return (cmp == 1);
}

template<class T>
bool operator ==(const Huge<T>& lhs, const Huge<T>& rhs)
{
    short cmp = compare(lhs, rhs);

    return (cmp == 0);
}

template<class T>
bool operator <=(const Huge<T>& lhs, const Huge<T>& rhs)
{
    short cmp = compare(lhs, rhs);

    return ((cmp == -1) || (cmp == 0));
}

template<class T>
bool operator >=(const Huge<T>& lhs, const Huge<T>& rhs)
{
    short cmp = compare(lhs, rhs);

    return ((cmp == 1) || (cmp == 0));
}

template<class T>
bool operator !=(const Huge<T>& lhs, const Huge<T>& rhs)
{
    short cmp = compare(lhs, rhs);

    return (cmp != 0);
}

template<class X>
Huge<X> operator +(const Huge<X>& lhs, const Huge<X>& rhs)
{
    // если знаки аргументов различны: (a)+(-b), (-a)+(b) ==> ?(a-b)
    if (lhs.m_Negative ^ rhs.m_Negative)
    {
        short cmp = HUGE_Compare(&(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

        if (cmp == -1)
        {    // (|a| < |b|) ==> (|b| - |a|)
            Huge<X> temp(rhs);

            HUGE_Subtract(&(*std::end(temp.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)));

            return temp;
        }
        else if (cmp == +1)
        {
            // (|a| > |b|) ==> (|a| - |b|)
            Huge<X> temp(lhs);

            HUGE_Subtract(&(*std::end(temp.m_Buffer)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

            return temp;
        }
    }
    else
    {    // если знаки аргументов одинаковы
        Huge<X> temp(std::max(lhs, rhs));

        HUGE_Add(&(*std::end(temp.m_Buffer)), &(*std::begin(lhs.m_Buffer)), &(*std::end(lhs.m_Buffer)), &(*std::begin(rhs.m_Buffer)), &(*std::end(rhs.m_Buffer)));

        temp.m_Negative = lhs.m_Negative & rhs.m_Negative;

        return temp;
    }

    return Huge<X>();
}

template<class X>
Huge<X> operator -(const Huge<X>& lhs, const Huge<X>& rhs)
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
    {    // (|a| < |b|) ==> (|b| - |a|)
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

template<class X>
Huge<X> operator *(const Huge<X>&, const Huge<X>&)
{
    return Huge<X>();
}

template<class X>
Huge<X> operator /(const Huge<X>&, const Huge<X>&)
{
    return Huge<X>();
}

template<class X>
Huge<X> operator %(const Huge<X>&, const Huge<X>&)
{
    return Huge<X>();
}

#endif

