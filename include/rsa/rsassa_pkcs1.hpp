//
// Created by max on 07.07.17.
//

#ifndef RSASSA_PKCS1_HPP
#define RSASSA_PKCS1_HPP

#include "basic_integer.hpp"

namespace cry
{

    namespace rsa
    {
        template <class Encoder = rsa::emsa_pkcs1<>, class Integer = bigint_t>
        struct rsassa_pkcs1
        {
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
            static OutputIterator sign(InputIterator first, InputIterator last, OutputIterator result, const Integer& d, const Integer& n, size_t modBits)
            {
                const auto emLen = (modBits + 7) / 8;
                std::vector<uint8_t> encoded(emLen);
                auto end = Encoder::encode(first, last, encoded.begin(), emLen);

                const Integer arg = OS2IP<Integer>()(encoded.begin(), encoded.end());
                const Integer s   = cry::pow_mod(arg, d, n);

                const std::vector<uint8_t> S = I2OSP<Integer>()(s);

                result = std::copy(S.begin(), S.end(), result);

                return result;
            }

            /**
             * \brief
             * \tparam InputIterator
             * \param s_first
             * \param s_last
             * \param m_first
             * \param m_last
             * \param e
             * \param n
             * \param modulusBits
             * \return
             */
            template <class InputIterator>
            static bool verify(InputIterator s_first, InputIterator s_last, InputIterator m_first, InputIterator m_last, const Integer& e, const Integer& n, size_t modulusBits)
            {

                ///////////////////////
                // 1. Length checking:
                const auto k = (modulusBits + 7) / 8;
                auto sz      = std::distance(s_first, s_last);
                if (k != sz)
                {
                    return false;
                }

                ////////////////////////
                // 2. RSA verification:

                //////////////////////////////////////////////////////////////////////
                // 2a. Convert the signature S to an integer signature representative
                const Integer s = OS2IP<Integer>()(s_first, s_last);

                ///////////////////////////////////////////////
                // 2b. Apply the RSAVP1 verification primitive
                const Integer m = cry::pow_mod(s, e, n);

                ////////////////////////////////////////////////////////////////////////////////////////
                // 2c. Convert the message representative m to an encoded message EM of length k octets
                const std::vector<uint8_t> EM = I2OSP<Integer>()(m);

                ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // 3. Apply the EMSA-PKCS1-v1_5 encoding operation to the message M to produce a second encoded message EM� of length k octets:
                std::vector<uint8_t> EM_(k);
                Encoder::encode(m_first, m_last, EM_.begin(), k);

                ////////////////////////////////////////////////////////////////////////
                // 4. Compare the encoded message EM and the second encoded message EM�
                auto it(EM_.begin());

                if (EM_.size() != EM.size())
                {
                    ++it;
                }

                auto f = std::equal(it, std::end(EM_), std::begin(EM), std::end(EM));

                return f;
            }
        };
    } // namespace rsa

} // namespace cry

#endif // CRY_RSA_H
