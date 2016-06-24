
#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

namespace cry {

template <class T>
T Gcd(const T& lhs, const T& rhs) 
{
  auto pair = std::minmax(lhs, rhs);

    auto r1(pair.second);
    auto r2(pair.first);

    while (r2 != 0)
    {
        auto rem = r1 % r2;
        r1             = r2;
        r2             = rem;
    }

    return r1;
}

template <class T>
bool ModInverse(T& inverse, const T& a, const T& modulus) 
{
  if (a >= modulus)
        return false;

    T r1(modulus);
    T r2(a);

    T t1 = 0;
    T t2 = 1;

    while (r2 > 0)
    {
        T q;
        T r;

        r1.DivRem(q, r, r2);
        r1 = r2;
        r2 = r;

        T t = (t1 - q * t2);
        t1        = t2;
        t2        = t;
    }

    if (r1 != 1)
        return false;

    if (t1 < 0)
    {
        t1 += modulus;
    }

    inverse = t1;

    return true;
}

template <class T>
bool IsEven(const T& arg)
{  
  bool f = (arg % 2) == 0; 
  return f;
}

template <class T>
bool IsOdd(const T& arg)
{    
  bool f = (arg % 2) == 1; 
    
  return f;
}

}

#endif