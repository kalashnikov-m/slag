
#ifndef HUGE_H
#define HUGE_H

#include <vector>

using namespace std;

typedef unsigned char byte;

template<class T>
class Huge
{
    public:

        Huge()
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
            m_Buffer(il)
        {
        }

        Huge& operator = (const Huge& other)
        {
            cout << "Huge<T>& Huge<T>::operator = (const Huge<T>& other)" << "\n";

            if (this != &other)
            {
                Huge<T> temp(other);

                temp.swap(*this);
            }

            return *this;

        }

        Huge& operator <<(int);

        Huge& operator >>(int);

        bool operator <(const Huge&);

        bool operator >(const Huge&);

        bool operator ==(const Huge&);

        bool operator !=(const Huge&);

        Huge& operator <<= (int);

        Huge& operator >>= (int);

        Huge& operator ++(int);

        Huge& operator --(int);

        Huge& operator += (const Huge&);

        Huge& operator -= (const Huge&);

        Huge& operator +();

        Huge operator -();

        // Huge& operator=(Huge&& other);

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
        void swap(Huge&) throw ()
        {
            cout << "Huge<T>::swap(Huge<T>& other)" << "\n";
        }

    private:

        std::vector<T> m_Buffer;
};


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
bool Huge<T>::operator <(const Huge<T>&)
{
    return false;
}

template<class T>
bool Huge<T>::operator >(const Huge<T>&)
{
    return false;
}

template<class T>
bool Huge<T>::operator ==(const Huge<T>&)
{
    return false;
}

template<class T>
Huge<T>& Huge<T>::operator <<= (int)
{
    return *this;
}

template<class T>
Huge<T>& Huge<T>::operator >>= (int)
{
    return *this;
}

template<class T>
Huge<T>& Huge<T>::operator ++(int)
{
    return *this;
}

template<class T>
Huge<T>& Huge<T>::operator --(int)
{
    return *this;
}

template<class T>
Huge<T>& Huge<T>::operator += (const Huge&)
{
    return *this;
}

template<class T>
Huge<T>& Huge<T>::operator -= (const Huge&)
{
    return *this;
}

template<class T>
Huge<T>& Huge<T>::operator +()
{
    return *this;
}

template<class T>
Huge<T> Huge<T>::operator -()
{
    return Huge<T>();
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


//~ Formatted by Jindent --- http://www.jindent.com
