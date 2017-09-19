//
// Created by max on 07.07.17.
//

#ifndef RSAES_OAEP_H
#define RSAES_OAEP_H

#include "algorithm.hpp"
#include "basic_int.hpp"
#include "os2ip.hpp"

namespace cry
{

    template <class Encoder = eme_oaep<>, class IntType = bigint8_t>
    struct rsaes_oaep
    {
        template <class InputIterator, class OutputIterator>
        static OutputIterator encrypt(InputIterator first, InputIterator last, OutputIterator result, const IntType& e, const IntType& n, size_t modBits, const std::vector<uint8_t>& seed = std::vector<uint8_t>(), const std::vector<uint8_t>& label = std::vector<uint8_t>())
        {

            ///////////////////////
            // 1. Length checking:

            ////////////////////////////////////////////////////////////////
            // If mLen > k – 2hLen – 2, output “message too long” and stop.

            size_t k    = modBits / 8;
            size_t mLen = std::distance(first, last);

            if (mLen > k - 2 * Encoder::hash_type::size - 2)
            {
                throw std::runtime_error("message too long");
            }

            /////////////////////////
            // 2. EME-OAEP encoding
            std::vector<uint8_t> EM(k);
            Encoder::encode(first, last, EM.begin(), k, seed, label);

            ///////////////////////
            // 3. RSA encryption:

            ////////////////////////////////////////////////////////////////////////////
            // a. Convert the encoded message EM to an integer message representative m
			IntType m = os2ip<IntType>()(EM);

            //////////////////////////////////////////////////////////////////////////
            // b. Apply the RSAEP encryption primitiveto the RSA public key(n, e) and
            // the message representative m to produce an integer ciphertext
            // representative c :

            IntType c = cry::pow_mod(m, e, n);

            ///////////////////////////////////////////////////////////////////////////////////
            // c. Convert the ciphertext representative c to a ciphertext C of length koctets

            std::vector<uint8_t> C = ip2os<IntType>()(c);

            result = std::copy(C.begin(), C.end(), result);

            return result;
        }

        template <class InputIterator, class OutputIterator>
        static OutputIterator decrypt(InputIterator c_first, InputIterator c_last, OutputIterator result, const IntType& d, const IntType& n, size_t modBits)
        {

            ///////////////////////
            // 1. Length checking:

            size_t k    = modBits / 8;
            size_t cLen = std::distance(c_first, c_last);

            ////////////////////////////////////////////////////////////////////////////////////////////
            // b. If the length of the ciphertext C is not k octets, output “decryption error” and stop.
            if (cLen != k)
            {
                throw std::runtime_error("decryption error");
            }

            ///////////////////////////////////////////////////////////
            // c. If k < 2hLen + 2, output “decryption error” and stop.

            if (k < 2 * Encoder::hash_type::size + 2)
            {
                throw std::runtime_error("decryption error");
            }

            //////////////////////
            // 2. RSA decryption:

            /////////////////////////////////////////////////////////////////////////
            // a. Convert the ciphertext C to an integer ciphertext representative c
            IntType c(c_first, c_last);

            //////////////////////////////////////////////////////////////////////////////
            // b. Apply the RSADP decryption primitive to the RSA private key K and the
            // ciphertext representative c to produce an integer message representative m:
            IntType m = cry::pow_mod(c, d, n);

            ////////////////////////////////////////////////////////////////////////////////////////
            // c. Convert the message representative m to an encoded message EM of length k octets:
            std::vector<uint8_t> EM = ip2os<IntType>()(m);

            ///////////////////////////
            // 3. EME - OAEP decoding:

            std::vector<uint8_t> M(k);
            auto m_end = Encoder::decode(EM.begin(), EM.end(), M.begin(), k);

            result = std::copy(M.begin(), m_end, result);

            return result;
        }
    };

} // namespace cry

#endif // CRY_RSA_H
