#ifndef EMSA_PKCS1_HPP
#define EMSA_PKCS1_HPP

#include "oid.hpp"

namespace cry
{
    namespace rsa
    {
        template <class Digest = sha1>
        class emsa_pkcs1
        {

          public:
            template <class InputIterator, class OutputIterator>
            static OutputIterator encode(InputIterator first, InputIterator last, OutputIterator result, size_t emLen)
            {

                std::vector<uint8_t> hash;
                hash.reserve(Digest::size);

                Digest()(first, last, std::back_inserter(hash));

                auto oid            = OID<Digest>::value();
                const size_t oidLen = oid.size();
                const size_t hLen   = hash.size();
                const size_t tLen   = hLen + oidLen;

                const size_t psLen = emLen - tLen - 3;

                std::vector<uint8_t> PS(psLen, 0xFF);

                *result++ = 0x00;
                *result++ = 0x01;

                result = std::copy(PS.begin(), PS.end(), result);

                *result++ = 0x00;

                result = std::copy(oid.begin(), oid.end(), result);

                result = std::copy(hash.begin(), hash.end(), result);

                return result;
            }

            template <class InputIterator, class OutputIterator>
            static OutputIterator decode(InputIterator first, InputIterator last, OutputIterator result, size_t emLen)
            {

                /*
                 * The format is
                 * 00 || 01 || PS || 00 || D
                 * PS - padding string, at least 8 bytes of FF
                 * D  - data.
                 */

                auto len = std::distance(first, last);
                if (len == emLen)
                {
                    if (first != last && *first++ != 0x00)
                        throw std::runtime_error("signature invalid");
                }

                if (first != last && *first++ != 0x01)
                    throw std::runtime_error("signature invalid");

                for (; first != last && *first != 0x00 && *first == 0xff; ++first)
                    ;

                if (*first++ != 0x00)
                    throw std::runtime_error("signature invalid");

                auto oid = OID<Digest>::value();

                const size_t oidLen = oid.size();
                bool eq             = std::equal(std::begin(oid), std::end(oid), first);
                if (!eq)
                {
                    throw std::runtime_error("signature invalid");
                }

                first += oidLen;

                result = std::copy(first, last, result);

                return result;
            }
        };
    } // namespace rsa
} // namespace cry

#endif
