//
// Created by max on 07.07.17.
//

#ifndef RSAES_PKCS1_H
#define RSAES_PKCS1_H

#include "algorithm.hpp"
#include "basic_integer.hpp"
#include "eme_pkcs1.hpp"
#include "os2ip.hpp"

namespace cry
{
    namespace rsa
    {
        template <class Integer = bigint_t>
        struct rsaes_pkcs1
        {
            /**
             * \brief
             * \tparam InputIterator
             * \tparam OutputIterator
             * \param first
             * \param last
             * \param result
             * \param e
             * \param n
             * \param modBits
             * \return
             */
            template <class InputIterator, class OutputIterator>
            static OutputIterator encrypt(InputIterator first, InputIterator last, OutputIterator result, const Integer& e, const Integer& n, size_t modBits)
            {
                const auto k = (modBits + 7) / 8;

                /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // 1. Apply the EME-PKCS1-v1_5 encoding operation to the message M to produce an encoded message EM of length k�1 octets:
                std::vector<uint8_t> EM(k);
                eme_pkcs1::encode(first, last, EM.begin(), k);

                /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // 2. Convert the encoded message EM to an integer message representative m
                const Integer m = OS2IP<Integer>()(EM);

                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // 3. Apply the RSAEP encryption primitive to the public key(n, e) and the message representative m to produce an integer ciphertext representative c:
                const Integer c = cry::pow_mod(m, e, n);

                ///////////////////////////////////////////////////////////////////////////////////
                // 4. Convert the ciphertext representative c to a ciphertext C of length k octets
                const std::vector<uint8_t> C = I2OSP<Integer>()(c);

                result = std::copy(C.begin(), C.end(), result);

                return result;
            }

            /**
             * \brief
             * \tparam InputIterator
             * \tparam OutputIterator
             * \param first
             * \param last
             * \param result
             * \param d
             * \param n
             * \param modBits
             * \return
             */
            template <class InputIterator, class OutputIterator>
            static OutputIterator decrypt(InputIterator first, InputIterator last, OutputIterator result, const Integer& d, const Integer& n, size_t modBits)
            {
                const auto k = (modBits + 7) / 8;

                ////////////////////////////////////////////////////////////////////////////////////////////
                // 1. If the length of the ciphertext C is not k octets, output �decryption error� and stop.
                auto cLen = std::distance(first, last);
                if (cLen != k)
                {
                    throw std::runtime_error("decryption error");
                }

                /////////////////////////////////////////////////////////////////////////
                // 2. Convert the ciphertext C to an integer ciphertext representative c:
                const Integer c = OS2IP<Integer>()(first, last);

                ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // 3. Apply the RSADP decryption primitive to the private key(n, d) and the ciphertext representative c to produce an integer message representative m:
                const Integer m = cry::pow_mod(c, d, n);

                /////////////////////////////////////////////////////////////////////////////////////////
                // 4. Convert the message representative m to an encoded message EM of length k�1 octets:
                const std::vector<uint8_t> EM = I2OSP<Integer>()(m);

				//////////////////////////////////////////////////
				// 5. Apply EME - PKCS1 - v1_5 decoding operation
                result = eme_pkcs1::decode(EM.begin(), EM.end(), result);

                return result;
            }
        };
    } // namespace rsa

} // namespace cry

#endif // CRY_RSA_H
