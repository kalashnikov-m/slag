//
// Created by max on 07.07.17.
//

#ifndef RSAES_PKCS1_H
#define RSAES_PKCS1_H

#include "basic_int.hpp"

namespace cry
{

    template <class Encoder, class IntType = bigint8_t>
    struct rsaes_pkcs1
    {
        template <class InputIterator, class OutputIterator>
        static OutputIterator encrypt(InputIterator first, InputIterator last, OutputIterator result, const IntType& e, const IntType& n, size_t modBits)
        {

            /*auto encoded = Encoder::encode(first, last, modulusBits / 8);

            IntType arg(encoded.begin(), encoded.end());
            IntType result = cry::pow_mod(arg, privateExponent, modulus);

            signature = (std::vector<uint8_t>)result;*/

            return result;
        }

        template <class InputIterator, class OutputIterator>
        static OutputIterator decrypt(InputIterator first, InputIterator last, OutputIterator result, const IntType& d, const IntType& n, size_t modBits)
        {

            return result;
        }
    };

} // namespace cry

#endif // CRY_RSA_H
