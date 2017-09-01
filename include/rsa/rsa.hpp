//
// Created by max on 07.07.17.
//

#ifndef RSA_H
#define RSA_H

#include <algorithm.hpp>

#include "basic_int.hpp"

namespace cry {

    template <class T = bigint8_t>
    class rsa {

    public:
        struct rsa_key
        {
            T exponent;
            T modulus;
        };

    public:
        void generate_key_pair(const T& p, const T& q)
        {

        }

        void generate_key_pair(rsa_key& pub, rsa_key& prv, const T& e, uint32_t modulusbits)
        {
            T p, q;

            p = cry::generate_probably_prime<T>(modulusbits, e);
            q = cry::generate_probably_prime<T>(modulusbits, e);

            std::cout<<"p: "<<p<<std::endl;
            std::cout<<"q: "<<q<<std::endl;

            T N = p*q;

            std::cout<<"N: "<<N<<std::endl;

            T Phi = (p - 1)*(q-1);
            std::cout<<"Phi: "<<Phi<<std::endl;

            T d;
            cry::mod_inverse(d, T(e), Phi);
            std::cout<<"d: "<<d<<std::endl;

            pub.exponent = e;
            pub.modulus = N;

            prv.exponent = d;
            prv.modulus = N;
        }      
    };
}


#endif //CRY_RSA_H
