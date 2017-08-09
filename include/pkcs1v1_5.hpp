#ifndef PKCS1_V1_5_HPP
#define PKCS1_V1_5_HPP

#include <random>
#include <algorithm>
#include "oid.hpp"

namespace cry {

    class EME_PKCS1_v1_5 {

      public:
        template <class InputIterator, class OutputIterator>
        void Encode(InputIterator first, InputIterator last, OutputIterator result, size_t k) {

            size_t mLen = std::distance(first, last);
            size_t psLen = k - mLen - 3;

            *result++ = 0x00;
            *result++ = 0x02;

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> uid(1, 255);

            std::generate_n(result, psLen, [&uid, &gen]() { return uid(gen); });
            result += psLen;

            *result++ = 0x00;

            std::copy(first, last, result);
        }

        template <class InputIterator, class OutputIterator>
        void Decode(InputIterator first, InputIterator last, OutputIterator result, size_t k) {
            if (first != last && *first++ != 0x00)
                throw 1; // decryption error

            if (first != last && *first++ != 0x02)
                throw 1; // decryption error

            for (; first != last && *first != 0x00; ++first)
                ;

            if (*first != 0x00)
                throw 1; // decryption error

            std::copy(first, last, result);
        }

      private:
    };

    class EMSA_PKCS1_v1_5 {

      public:
        template <class DigestType, class InputIterator, class OutputIterator>
        void Encode(InputIterator first, InputIterator last, OutputIterator result, size_t emLen) {

            std::vector<uint8_t> hash(DigestType::size);

            DigestType digest;
            digest.Init();
            digest.Update(first, last);
            digest.Final(hash.begin());

            auto oid = OID<DigestType>::value();
            size_t oidLen = oid.size();
            size_t hLen = hash.size();
            size_t tLen = hLen + oidLen;

            size_t psLen = emLen - tLen - 3;

            std::vector<uint8_t> PS(psLen, 0xFF);

            *result++ = 0x00;
            *result++ = 0x01;

            std::copy(PS.begin(), PS.end(), result);
            result += psLen;

            *result++ = 0x00;

            std::copy(oid.begin(), oid.end(), result);
            result += oidLen;

            std::copy(hash.begin(), hash.end(), result);
        }

        template <class DigestType, class InputIterator, class OutputIterator>
        void Decode(InputIterator first, InputIterator last, OutputIterator result, size_t k) {
            if (first != last && *first++ != 0x00)
                throw 1; // signature invalid

            if (first != last && *first++ != 0x01)
                throw 1; // decryption error

            for (; first != last && *first != 0x00; ++first)
                ;

            if (*first++ != 0x00)
                throw 1; // signature invalid

            auto oid = OID<DigestType>::value();

            size_t oidLen = std::distance(std::begin(oid), std::end(oid));
            bool eq = std::equal(std::begin(oid), std::end(oid), first);
            if (!eq) {
                throw 1; // signature invalid
            }

            first += oidLen;

            std::copy(first, last, result);
        }

      private:
    };
}

#endif
