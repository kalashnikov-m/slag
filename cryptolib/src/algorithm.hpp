
#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

namespace cry
{
template<class T>
T Gcd(const T& lhs, const T& rhs)
{
    auto pair = std::minmax(lhs, rhs);

    auto r1(pair.second);
    auto r2(pair.first);

    while (r2 != 0)
    {
        auto rem = r1 % r2;
        r1       = r2;
        r2       = rem;
    }

    return r1;
}

template<class T>
bool ModInverse(T& inverse, const T& a, const T& modulus)
{
    if (a >= modulus)
    {
        return false;
    }

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

        t1 = t2;
        t2 = t;
    }

    if (r1 != 1)
    {
        return false;
    }

    if (t1 < 0)
    {
        t1 += modulus;
    }

    inverse = t1;

    return true;
}

template<class T>
bool IsEven(const T& arg)
{
    bool f = (arg % 2) == 0;

    return f;
}

template<class T>
bool IsOdd(const T& arg)
{
    bool f = (arg % 2) == 1;

    return f;
}

template<class T>
const T Pow(const T& arg, const T& exp)
{
    T y = 1;
    T a = arg;
    T e = exp;

    while (e > 0)
    {
        if (cry::IsOdd(e))
        {
            y *= a;
        }

        a *= a;
        e >>= 1;
    }

    return y;
}

template<class T>
const T PowMod(const T& arg, const T& exp, const T& mod)
{
    T y = 1;
    T a = arg;
    T e = exp;

    while (e > 0)
    {
        if (e % 2 == 1)
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

template<class T>
bool IsProbablyPrime(const T& p, int t)
{
    if (p % 2 == 0)
    {
        return false;
    }

    auto p_minus_1 = p - 1;
    int v = 0;
    auto w = p_minus_1;
    
    while (w % 2 == 0)
      {
        v++;
        w /= 2;
      }
    
    int a = 1;
    
    nexta;
    for (;t-- > 0;)
      {
        a++;
        
        auto b = PowMod(a, w, p);
        if (b ==1 || b == p_minus_1)
          {
            goto nexta;
          }
        
        for (int j = 1; j < v; ++j)
          {
            x = PowMod(x, 2, p);
            if (x == 1)
              {
                return false;
              }
            
            if (x == p_minus_1)
              {
                goto nexta;
              }
          }
        
        return false;
        
      }
           
    return true;
}

}

#endif

