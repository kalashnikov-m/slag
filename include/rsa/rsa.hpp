//
// Created by max on 07.07.17.
//

#ifndef RSA_H
#define RSA_H

#include <algorithm.hpp>

#include "basic_integer.hpp"

namespace cry
{
    namespace rsa
    {
        template <class T>
        void generate_key_pair(T& n, T& d, uint32_t e, uint32_t modulusbits)
        {
            for (;;)
            {
                T p, q;
                cry::generate_probably_prime<T>(std::ref(p), modulusbits / 2, e);
                cry::generate_probably_prime<T>(std::ref(q), modulusbits / 2, e);

                T N   = p * q;
                T Phi = (p - 1) * (q - 1);

                T D;

                const bool f = cry::mod_inverse(D, T(e), Phi);
                if (f)
                {
                    n = N;
                    d = D;

                    break;
                }
            }
        }

        template <class T>
        void generate_key_pair_mt(T& n, T& d, uint32_t e, uint32_t modulusbits)
        {
            for (;;)
            {
                T p, q;

                std::thread pThread(cry::generate_probably_prime<T>, std::ref(p), modulusbits / 2, e);
                std::thread qThread(cry::generate_probably_prime<T>, std::ref(q), modulusbits / 2, e);

                pThread.join();
                qThread.join();

                T N   = p * q;
                T Phi = (p - 1) * (q - 1);

                T D;
                const bool f = cry::mod_inverse(D, T(e), Phi);
                if (f)
                {
                    n = N;
                    d = D;
                    break;
                }
            }
        }
    }
}

#endif // CRY_RSA_HPP
