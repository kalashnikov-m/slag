
#ifndef HUGE_H
#define HUGE_H

#include <vector>
#include <algorithm>

#include "hugecore.h"

using namespace std;

typedef unsigned char byte;

template<class T>
class Huge
{
    public:

        Huge():
            m_Negative(false)
        {
            cout << "Huge<T>::Huge()" << "\n";
        }

        ~Huge()
        {
        }

        Huge(const Huge& other)
        {
            cout << "Huge<T>::Huge(const Huge<T>& other);" << "\n";
        }

        // HugeInt(HugeInt&& other);

        Huge(const std::initializer_list<T>& il):
            m_Buffer(il),
            m_Negative(false)
        {
        }

        Huge& operator = (const Huge& other)
        {
            cout << "Huge<T>& Huge<T>::operator = (const Huge<T>& other)" << "\n";

            if (this != &other)
            {
                Huge<T> temp(other);

                temp.__swap(*this);
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
        {    /* TODO */
        }

        Huge operator -()
        {    /* TODO */
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
        {    /* TODO */
        }

        Huge operator %= (const Huge&)
        {    /* TODO */
        }

        Huge operator &()
        {    /* TODO */
        }

        Huge operator &= (const Huge&)
        {    /* TODO */
        }

        Huge operator |(const Huge&)
        {    /* TODO */
        }

        Huge operator |= (const Huge&)
        {    /* TODO */
        }

        Huge operator ^(const Huge&)
        {    /* TODO */
        }

        Huge operator ^= (const Huge&)
        {    /* TODO */
        }

        Huge operator *= (const Huge&)
        {    /* TODO */
        }

        Huge operator /= (const Huge&)
        {    /* TODO */
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
        void __swap(Huge&) throw ()
        {
            cout << "Huge<T>::swap(Huge<T>& other)" << "\n";
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

