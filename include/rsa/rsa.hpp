//
// Created by max on 07.07.17.
//

#ifndef RSA_H
#define RSA_H

#include <algorithm.hpp>

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
            cry::ModInverse(d, T(e), Phi);
            std::cout<<"d: "<<d<<std::endl;

            pub.exponent = e;
            pub.modulus = N;

            prv.exponent = d;
            prv.modulus = N;
        }

        void encrypt(T& encrypted, const T& plain, const rsa_key& public_key)
        {
            encrypted = cry::PowMod(plain, public_key.exponent, public_key.modulus);
        }

        void decrypt(T& plain, const T& encrypted, const rsa_key& private_key)
        {
            plain = cry::PowMod(encrypted, private_key.exponent, private_key.modulus);
        }

        void sign(T& signature, const T& plain, const rsa_key& private_key)
        {
            signature = cry::PowMod(plain, private_key.exponent, private_key.modulus);
        }

        /*void verify(std::vector<uint8_t>& plain, const std::vector<uint8_t> signature, const rsa_key& public_key)
        {

        }*/


    };
}


#endif //CRY_RSA_H