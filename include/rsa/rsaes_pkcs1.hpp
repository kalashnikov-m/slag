//
// Created by max on 07.07.17.
//

#ifndef RSAES_PKCS1_H
#define RSAES_PKCS1_H

#include "os2ip.hpp"
#include "algorithm.hpp"
#include "basic_int.hpp"
#include "eme_pkcs1.hpp"

namespace cry
{

    template <class Encoder = eme_pkcs1, class IntType = bigint8_t>
    struct rsaes_pkcs1
    {
        template <class InputIterator, class OutputIterator>
        static OutputIterator encrypt(InputIterator first, InputIterator last, OutputIterator result, const IntType& e, const IntType& n, size_t modBits)
        {
            size_t k = modBits / 8;
            if (modBits % 8 != 0)
                k++;

            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // 1. Apply the EME-PKCS1-v1_5 encoding operation to the message M to produce an encoded message EM of length k–1 octets:
            std::vector<uint8_t> EM(k);
            Encoder::encode(first, last, EM.begin(), k - 1);

            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // 2. Convert the encoded message EM to an integer message representative m
            const IntType m = OS2IP<IntType>()(EM);

            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // 3. Apply the RSAEP encryption primitive to the public key(n, e) and the message representative m to produce an integer ciphertext representative c:
            const IntType c = cry::pow_mod(m, e, n);

            ///////////////////////////////////////////////////////////////////////////////////
            // 4. Convert the ciphertext representative c to a ciphertext C of length k octets
            const std::vector<uint8_t> C = IP2OS<IntType>()(c);

            result = std::copy(C.begin(), C.end(), result);

            return result;
        }

        template <class InputIterator, class OutputIterator>
        static OutputIterator decrypt(InputIterator first, InputIterator last, OutputIterator result, const IntType& d, const IntType& n, size_t modBits)
        {
            size_t k = modBits / 8;
            if (modBits % 8 != 0)
                k++;

            ////////////////////////////////////////////////////////////////////////////////////////////
            // 1. If the length of the ciphertext C is not k octets, output “decryption error” and stop.
            auto cLen = std::distance(first, last);
            if (cLen != k)
            {
                throw std::runtime_error("decryption error");
            }

            /////////////////////////////////////////////////////////////////////////
            // 2. Convert the ciphertext C to an integer ciphertext representative c:
            const IntType c = OS2IP<IntType>()(first, last);

            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // 3. Apply the RSADP decryption primitive to the private key(n, d) and the ciphertext representative c to produce an integer message representative m:
            const IntType m = cry::pow_mod(c, d, n);

            /////////////////////////////////////////////////////////////////////////////////////////
            // 4. Convert the message representative m to an encoded message EM of length k–1 octets:
            const std::vector<uint8_t> EM = IP2OS<IntType>()(m);

            result = Encoder::decode(EM.begin(), EM.end(), result);

            return result;
        }
    };

} // namespace cry

#endif // CRY_RSA_H
