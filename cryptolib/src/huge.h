
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

        template<class X>
        friend Huge& operator <<(Huge&, int);

        template<class X>
        friend Huge& operator >>(Huge&, int);

        template<class X>
        friend bool operator <(const Huge&, const Huge&);

        template<class X>
        friend bool operator >(const Huge&, const Huge&);

        template<class X>
        friend bool operator ==(const Huge&, const Huge&);

    protected:
        void swap(Huge&) throw ()
        {
            cout << "Huge<T>::swap(Huge<T>& other)" << "\n";
        }

    private:

        T m_Buffer;
};


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

template<class X>
Huge<X>& operator <<(Huge<X>& self, int)
{
    return self;
}

template<class X>
Huge<X>& operator >>(Huge<X>& self, int)
{
    return self;
}

template<class X>
bool operator <(const Huge<X>&, const Huge<X>&)
{
    return false;
}

template<class X>
bool operator >(const Huge<X>&, const Huge<X>&)
{
    return false;
}

template<class X>
bool operator ==(const Huge<X>&, const Huge<X>&)
{
    return false;
}

#endif


//~ Formatted by Jindent --- http://www.jindent.com
