#ifndef PSS_H
#define PSS_H

#include <mgf.hpp>

namespace cry {

    template <class HashType, class MGFType = MGF1<SHA1>, size_t sLen = HashType::size>
    class PSS {

      public:
        template <class InputIterator, class OutputIterator>
        void Encode(InputIterator first, InputIterator last, OutputIterator result, size_t emBits) const {

            size_t emLen = emBits / 8;
            size_t hLen = HashType::size;
            size_t zBits = 8 * emLen - emBits;

            if (emLen < hLen + sLen + 2) {
                throw 2; // encoding error
            }

            HashType hash;

            std::vector<uint8_t> mHash(hLen);
            hash(first, last, mHash.begin());

            std::vector<uint8_t> salt(sLen);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> uid(1, 255);

            std::generate(std::begin(salt), std::end(salt), [&uid, &gen]() { return uid(gen); });

            std::vector<uint8_t> M_(8 + hLen + sLen, 0x00);
            std::vector<uint8_t>::iterator it(M_.begin());

            it += 8;

            std::copy(mHash.begin(), mHash.end(), it);
            it += hLen;

            std::copy(salt.begin(), salt.end(), it);
            it += sLen;

            std::vector<uint8_t> H(hLen);
            hash(M_.begin(), M_.end(), H.begin());

            size_t psLen = emLen - sLen - hLen - 2;
            std::vector<uint8_t> PS(psLen);

            size_t dbLen = emLen - hLen - 1;
            std::vector<uint8_t> DB(dbLen);
            std::vector<uint8_t>::iterator itDb(DB.begin());

            std::copy(PS.begin(), PS.end(), itDb);
            itDb += psLen;

            *itDb++ = 0x01;

            std::copy(salt.begin(), salt.end(), itDb);
            itDb += sLen;

            // 9. Let dbMask = MGF (emLen - hLen - 1)
            std::vector<uint8_t> dbMask(/*emLen - hLen - 1*/ dbLen);

            MGFType mgf;
            mgf(H.begin(), H.end(), dbMask.begin(), /*emLen - hLen - 1*/ dbLen);

            // 10. Let maskedDB = DB (+) dbMask
            std::vector<uint8_t> maskedDB(dbLen);
            for (size_t i = 0; i < dbLen; ++i) {
                maskedDB[i] = DB[i] ^ dbMask[i];
            }

            if (zBits > 0)
                *maskedDB.begin() &= (0xFF >> (8 - zBits));

            std::copy(maskedDB.begin(), maskedDB.end(), result);
            result += dbLen;

            std::copy(H.begin(), H.end(), result);
            result += hLen;

            *result++ = 0xbc;
        }

        template <class InputIterator, class OutputIterator>
        void Decode(InputIterator first, InputIterator last, OutputIterator result) const {}
    };
}

#endif
