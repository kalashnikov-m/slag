
#ifndef HUGE_H
#define HUGE_H

#include <vector>
#include <algorithm>

#include "HugeCore.h"

using namespace std;

typedef unsigned char byte;

template<class T>
class Huge
{
    public:

        Huge():
            m_Negative(false)
        {
        }

        ~Huge()
        {
        }

        Huge(const Huge& other):
            m_Buffer(other.m_Buffer),
            m_Negative(other.m_Negative)
        {
        }

        // HugeInt(HugeInt&& other);

        Huge(const std::initializer_list<T>& il, bool negative = false):
            m_Buffer(il),
            m_Negative(negative)
        {
        }

        Huge(const std::vector<T>& iv, bool negative = false):
            m_Buffer(iv),
            m_Negative(negative)
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

        Huge& operator <<(int);

        Huge& operator >>(int);

        bool operator <(const Huge&);

        bool operator >(const Huge&);

        bool operator <=(const Huge&);

        bool operator >=(const Huge&);

        bool operator ==(const Huge&);

        bool operator !=(const Huge&);

        Huge& operator <<= (int);

        Huge& operator >>= (int);

        Huge& operator ++()
        {    /* TODO */
        }

        Huge& operator --()
        {    /* TODO */
        }

        Huge operator ++(int)
        {    /* TODO */
        }

        Huge operator --(int)
        {    /* TODO */
        }

        Huge& operator += (const Huge&)
        {    /* TODO */
        }

        Huge& operator -= (const Huge&)
        {    /* TODO */
        }

        Huge& operator +()
        {
            return (*this);
        }

        Huge operator -()
        {
            return Huge(m_Buffer, !m_Negative);
        }

        Huge operator !()
        {    /* TODO */
        }

        Huge operator &&(const Huge&)
        {    /* TODO */
        }

        Huge operator ||(const Huge&)
        {    /* TODO */
        }

        Huge operator ~()
        {
            Huge ret(*this);

            auto *b = &(*ret.m_Buffer.begin());
            auto *e = &(*ret.m_Buffer.end());

            HUGE_Inverse(b, e);

            return ret;
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

            auto *er = &(*temp.end());
            auto *b1 = &(*m_Buffer.begin());
            auto *e1 = &(*m_Buffer.end());

            auto *b2 = &(*rhs.m_Buffer.begin());
            auto *e2 = &(*rhs.m_Buffer.end());

            HUGE_And(er, b1, e1, b2, e2);

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

            auto *er = &(*temp.end());
            auto *b1 = &(*m_Buffer.begin());
            auto *e1 = &(*m_Buffer.end());

            auto *b2 = &(*rhs.m_Buffer.begin());
            auto *e2 = &(*rhs.m_Buffer.end());

            HUGE_Or(er, b1, e1, b2, e2);

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

            auto *er = &(*temp.end());
            auto *b1 = &(*m_Buffer.begin());
            auto *e1 = &(*m_Buffer.end());

            auto *b2 = &(*rhs.m_Buffer.begin());
            auto *e2 = &(*rhs.m_Buffer.end());

            HUGE_Xor(er, b1, e1, b2, e2);

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
        friend Huge operator +(const Huge&, const Huge&);

        template<class X>
        friend Huge operator -(const Huge&, const Huge&);

        template<class X>
        friend Huge operator *(const Huge&, const Huge&);

        template<class X>
        friend Huge operator /(const Huge&, const Huge&);

        template<class X>
        friend Huge operator %(const Huge&, const Huge&);

    protected:
        void __swap(Huge& other) throw ()
        {
            m_Buffer.swap(other.m_Buffer);

            std::swap(m_Negative, other.m_Negative);
        }

        short __compare(const Huge& lhs, const Huge& rhs);

    private:

        std::vector<T> m_Buffer;
        bool           m_Negative;
};


template<class T>
short Huge<T>::__compare(const Huge<T>& lhs, const Huge<T>& rhs)
{
    auto *b1 = &(*lhs.m_Buffer.begin());
    auto *e1 = &(*lhs.m_Buffer.end());
    auto *b2 = &(*rhs.m_Buffer.begin());
    auto *e2 = &(*rhs.m_Buffer.end());

    short cmp = HUGE_Compare(b1, e1, b2, e2);

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
Huge<T>& Huge<T>::operator <<(int)
{
    return *this;
}

template<class T>
Huge<T>& Huge<T>::operator >>(int)
{
    return *this;
}

template<class T>
bool Huge<T>::operator <(const Huge<T>& rhs)
{
    short cmp = __compare(*this, rhs);

    return (cmp == -1);
}

template<class T>
bool Huge<T>::operator >(const Huge<T>& rhs)
{
    short cmp = __compare(*this, rhs);

    return (cmp == 1);
}

template<class T>
bool Huge<T>::operator ==(const Huge<T>& rhs)
{
    if (m_Negative != rhs.m_Negative)
    {
        return false;
    }

    short cmp = __compare(*this, rhs);

    return (cmp == 0);
}

template<class T>
bool Huge<T>::operator <=(const Huge<T>& rhs)
{
    short cmp = __compare(*this, rhs);

    return ((cmp == -1) || (cmp == 0));
}

template<class T>
bool Huge<T>::operator >=(const Huge<T>& rhs)
{
    short cmp = __compare(*this, rhs);

    return ((cmp == 1) || (cmp == 0));
}

template<class T>
bool Huge<T>::operator !=(const Huge<T>& rhs)
{
    short cmp = __compare(*this, rhs);

    return (cmp != 0);
}

template<class X>
Huge<X> operator +(const Huge<X>&, const Huge<X>&)
{
    return Huge<X>();
}

template<class X>
Huge<X> operator -(const Huge<X>&, const Huge<X>&)
{
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

