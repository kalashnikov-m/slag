#ifndef PKCS1_V1_5_HPP
#define PKCS1_V1_5_HPP

#include <random>

namespace cry {

    class EME_PKCS1_v1_5 {

      public:
        std::vector<uint8_t> Encode(const std::vector<uint8_t>& message, size_t k) {
            size_t mLen = message.size();
            size_t psLen = k - mLen - 3;

            std::vector<uint8_t> ps(psLen);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> uid(1, 255);

            std::generate(std::begin(ps), std::end(ps), [&uid, &gen]() { return uid(gen); });

            std::vector<uint8_t> em(k);
            std::vector<uint8_t>::iterator it = em.begin(), end(em.end());

            *it++ = 0x00;
            *it++ = 0x02;

            std::copy(ps.begin(), ps.end(), it);

            it += psLen;

            *it++ = 0x00;

            std::copy(message.begin(), message.end(), it);

            return em;
        }

        std::vector<uint8_t> Decode(const std::vector<uint8_t>& message, size_t k) { return std::vector<uint8_t>(); }

      private:
    };

    class EMSA_PKCS1_v1_5 {

      public:
        template <class DigestType>
        std::vector<uint8_t> Encode(const std::vector<uint8_t>& message, size_t emLen) {

            std::vector<uint8_t> hash;

            DigestType digest;
            digest.Init();
            digest.Update(message);
            digest.Final(hash);

            auto oid = OID<DigestType>::value();
            size_t oidLen = oid.size();
            size_t hLen = hash.size();
            size_t tLen = hLen + oidLen;

            size_t psLen = emLen - tLen - 3;

            std::vector<uint8_t> PS(psLen, 0xFF);

            std::vector<uint8_t> EM(emLen);

            std::vector<uint8_t>::iterator it(EM.begin()), end(EM.end());

            *it++ = 0x00;
            *it++ = 0x01;

            std::copy(PS.begin(), PS.end(), it);
            it += psLen;

            *it++ = 0x00;

            std::copy(oid.begin(), oid.end(), it);
            it += oidLen;

            std::copy(hash.begin(), hash.end(), it);

            return EM;
        }

        std::vector<uint8_t> Decode(const std::vector<uint8_t>& message, size_t k) { return std::vector<uint8_t>(); }

      private:
    };
}

#endif
