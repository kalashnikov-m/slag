//
// Created by max on 07.07.17.
//

#ifndef RSAPSS_PSS_H
#define RSAPSS_PSS_H

#include "basic_integer.hpp"
#include "emsa_pss.hpp"

namespace cry
{
    namespace rsa
    {

        template <class Digest = sha1, class MGFType = mgf1<sha1>, size_t sLen = Digest::size, class Integer = bigint_t>
        struct rsassa_pss
        {
            /**
             * \brief
             * \tparam InputIterator
             * \tparam OutputIterator
             * \param m_first
             * \param m_last
             * \param result
             * \param n
             * \param d
             * \param modBits
             * \param salt
             * \return
             */
            template <class InputIterator, class OutputIterator>
            static OutputIterator sign(InputIterator m_first, InputIterator m_last, OutputIterator result, const Integer& n, const Integer& d, size_t modBits, const vector<uint8_t>& salt = vector<uint8_t>())
            {
                //////////////////////////
                // 1. EMSA-PSS encoding:
                std::vector<uint8_t> EM(modBits / 8);
                emsa_pss<Digest, MGFType, sLen>::encode(m_first, m_last, EM.begin(), modBits - 1, salt);

                //////////////////////////
                // 2. RSA signature:

                /////////////////////////////////////////////////////////////////////////////
                // 2a. Convert the encoded message EM to an integer message representative m
                const Integer m = OS2IP<Integer>()(EM.begin(), EM.end());

                ////////////////////////////////////////////
                // 2b. Apply the RSASP1 signature primitive
                const Integer s = cry::pow_mod(m, d, n);

                //////////////////////////////////////////////////////////////////////////////////
                // 2c. Convert the signature representative s to a signature S of length k octets
                const std::vector<uint8_t> S = I2OSP<Integer>()(s);

                result = std::copy(S.begin(), S.end(), result);

                return result;
            }

            /**
             * \brief
             * \tparam MInputIterator
             * \tparam InputIterator
             * \param m_first
             * \param m_last
             * \param s_first
             * \param s_last
             * \param n
             * \param e
             * \param modBits
             * \return
             */
            template <class MInputIterator, class InputIterator>
            static bool verify(MInputIterator m_first, MInputIterator m_last, InputIterator s_first, InputIterator s_last, const Integer& n, const Integer& e, size_t modBits)
            {
                //////////////////////////////////////////
                // 1. Length checking:
                auto k = std::distance(s_first, s_last);
                if (k != modBits / 8)
                    return false;

                ///////////////////////////////////////
                // 2. RSA verification:

                ///////////////////////////////////////////////////////////////////////
                // 2a. Convert the signature S to an integer signature representative s
                const Integer s = OS2IP<Integer>()(s_first, s_last);

                ///////////////////////////////////////////////////////////////////////
                // 2b. Apply the RSAVP1 verification primitive to to produce an integer message representative m:
                const Integer m = cry::pow_mod(s, e, n);

                ///////////////////////////////////////////////////////////////////////
                // 2c. Convert the message representative m to an encoded message EM
                const std::vector<uint8_t> EM = I2OSP<Integer>()(m);

                //////////////////////////////
                // 3. EMSA - PSS verification
                const bool result = emsa_pss<Digest, MGFType, sLen>::verify(m_first, m_last, EM.begin(), EM.end(), modBits - 1);

                return result;
            }
        };
    } // namespace rsa
} // namespace cry

#endif // RSAPSS_PSS_H
