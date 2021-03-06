//
// Created by max on 07.07.17.
//

#ifndef RSAES_OAEP_H
#define RSAES_OAEP_H

#include "algorithm.hpp"
#include "basic_integer.hpp"
#include "eme_oaep.hpp"
#include "utility/os2ip.hpp"

namespace cry
{
    namespace rsa
    {
        template <class Digest = sha1, class MGFType = mgf1<sha1>, size_t hLen = Digest::size, class Integer = bigint_t>
        struct rsaes_oaep
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
             * \param seed
             * \param label
             * \return
             */
            template <class InputIterator, class OutputIterator>
            static OutputIterator encrypt(InputIterator first, InputIterator last, OutputIterator result, const Integer& e, const Integer& n, size_t modBits, const std::vector<uint8_t>& seed = std::vector<uint8_t>(), const std::vector<uint8_t>& label = std::vector<uint8_t>())
            {
                const auto k = (modBits + 7) / 8;

                /////////////////////////
                // 2. EME-OAEP encoding
                std::vector<uint8_t> EM(k);
                eme_oaep<Digest, MGFType, hLen>::encode(first, last, EM.begin(), k, seed, label);

                ///////////////////////
                // 3. RSA encryption:

                ////////////////////////////////////////////////////////////////////////////
                // a. Convert the encoded message EM to an integer message representative m
                Integer m = OS2IP<Integer>()(EM);

                //////////////////////////////////////////////////////////////////////////
                // b. Apply the RSAEP encryption primitiveto the RSA public key(n, e) and
                // the message representative m to produce an integer ciphertext
                // representative c :

                Integer c = cry::pow_mod(m, e, n);

                ///////////////////////////////////////////////////////////////////////////////////
                // c. Convert the ciphertext representative c to a ciphertext C of length koctets
                const std::vector<uint8_t> C = I2OSP<Integer>()(c);

                result = std::copy(C.begin(), C.end(), result);

                return result;
            }

            /**
             * \brief
             * \tparam InputIterator
             * \tparam OutputIterator
             * \param c_first
             * \param c_last
             * \param result
             * \param d
             * \param n
             * \param modBits
             * \return
             */
            template <class InputIterator, class OutputIterator>
            static OutputIterator decrypt(InputIterator c_first, InputIterator c_last, OutputIterator result, const Integer& d, const Integer& n, size_t modBits)
            {

                ///////////////////////
                // 1. Length checking:

                const auto k      = (modBits + 7) / 8;
                const size_t cLen = std::distance(c_first, c_last);

                ////////////////////////////////////////////////////////////////////////////////////////////
                // b. If the length of the ciphertext C is not k octets, output �decryption error� and stop.
                if (cLen != k)
                {
                    throw std::runtime_error("decryption error");
                }

                ///////////////////////////////////////////////////////////
                // c. If k < 2hLen + 2, output �decryption error� and stop.
                if (k < 2 * hLen + 2)
                {
                    throw std::runtime_error("decryption error");
                }

                //////////////////////
                // 2. RSA decryption:

                /////////////////////////////////////////////////////////////////////////
                // a. Convert the ciphertext C to an integer ciphertext representative c
                const Integer c = OS2IP<Integer>()(c_first, c_last);

                //////////////////////////////////////////////////////////////////////////////
                // b. Apply the RSADP decryption primitive to the RSA private key K and the
                // ciphertext representative c to produce an integer message representative m:
                const Integer m = cry::pow_mod(c, d, n);

                ////////////////////////////////////////////////////////////////////////////////////////
                // c. Convert the message representative m to an encoded message EM of length k octets:
                const std::vector<uint8_t> EM = I2OSP<Integer>()(m);

                ///////////////////////////
                // 3. EME - OAEP decoding:
                result = eme_oaep<Digest, MGFType, hLen>::decode(EM.begin(), EM.end(), result, k);

                return result;
            }
        };
    } // namespace rsa
} // namespace cry

#endif // CRY_RSA_H
