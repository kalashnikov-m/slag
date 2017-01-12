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

            if (emLen < hLen + sLen + 2) {
                throw 2; // encoding error
            }

            HashType hash;

            std::vector<uint8_t> mHash(hLen);
            hash(first, last, mHash.begin());

            /*for (int i = 0; i < hLen; ++i) {
                printf("%02x ", mHash[i]);
                if ((i + 1) % 16 == 0)
                    printf("\n");
            }

            printf("\n\n");*/

            std::vector<uint8_t> salt(sLen);

            /*salt.push_back(0xe3);
            salt.push_back(0xb5);
            salt.push_back(0xd5);
            salt.push_back(0xd0);
            salt.push_back(0x02);
            salt.push_back(0xc1);
            salt.push_back(0xbc);
            salt.push_back(0xe5);
            salt.push_back(0x0c);
            salt.push_back(0x2b);
            salt.push_back(0x65);
            salt.push_back(0xef);
            salt.push_back(0x88);
            salt.push_back(0xa1);
            salt.push_back(0x88);
            salt.push_back(0xd8);
            salt.push_back(0x3b);
            salt.push_back(0xce);
            salt.push_back(0x7e);
            salt.push_back(0x61);*/

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> uid(1, 255);

            std::generate(std::begin(salt), std::end(salt), [&uid, &gen]() { return uid(gen); });

            std::vector<uint8_t> M_(8 + hLen + sLen, 0x00);
            std::vector<uint8_t>::iterator it(M_.begin());

            it += 8;

            std::copy(mHash.begin(), mHash.end(), it);
            it += hLen;

            /*for (int i = 0; i < sLen; ++i) {
                printf("%02x ", salt[i]);
                if ((i + 1) % 16 == 0)
                    printf("\n");
            }

            printf("\n\n");*/

            std::copy(salt.begin(), salt.end(), it);
            it += sLen;

            /*for (int i = 0; i < 8 + hLen + sLen; ++i) {
                printf("%02x ", M_[i]);
                if ((i + 1) % 16 == 0)
                    printf("\n");
            }

            printf("\n\n");*/

            std::vector<uint8_t> H(hLen);
            hash(M_.begin(), M_.end(), H.begin());

            /*for (int i = 0; i < hLen; ++i) {
                printf("%02x ", H[i]);
                if ((i + 1) % 16 == 0)
                    printf("\n");
            }

            printf("\n\n");*/

            size_t psLen = emLen - sLen - hLen - 2;
            // printf("%d\n", psLen);
            std::vector<uint8_t> PS(psLen);

            /*for (int i = 0; i < psLen; ++i)
            {
                printf("%02x ", PS[i]);
            }

            printf("\n");*/

            size_t dbLen = emLen - hLen - 1;
            std::vector<uint8_t> DB(dbLen);
            std::vector<uint8_t>::iterator itDb(DB.begin());

            std::copy(PS.begin(), PS.end(), itDb);
            itDb += psLen;

            *itDb++ = 0x01;

            std::copy(salt.begin(), salt.end(), itDb);
            itDb += sLen;

            /*for (int i = 0; i < dbLen; ++i) {
                printf("%02x ", DB[i]);
                if ((i + 1) % 16 == 0)
                    printf("\n");
            }

            printf("\n\n");*/

            // 9. Let dbMask = MGF (emLen - hLen - 1)
            std::vector<uint8_t> dbMask(/*emLen - hLen - 1*/ dbLen);

            MGFType mgf;
            mgf(H.begin(), H.end(), dbMask.begin(), /*emLen - hLen - 1*/ dbLen);

            /*for (int i = 0; i < dbLen; ++i) {
                printf("%02x ", dbMask[i]);
                if ((i + 1) % 16 == 0)
                    printf("\n");
            }

            printf("\n\n");*/

            // 10. Let maskedDB = DB (+) dbMask
            std::vector<uint8_t> maskedDB(dbLen);
            for (size_t i = 0; i < dbLen; ++i) {
                maskedDB[i] = DB[i] ^ dbMask[i];
            }

            /*for (int i = 0; i <  dbLen; ++i) {
                printf("%02x ", maskedDB[i]);
                if ((i + 1) % 16 == 0)
                    printf("\n");
            }

            printf("\n\n");*/

            *maskedDB.begin() &= (~0x80);
            std::copy(maskedDB.begin(), maskedDB.end(), result);
            result += dbLen;

            std::copy(H.begin(), H.end(), result);
            result += hLen;

            *result++ = 0xbc;

            /*for (int i = 0; i < emLen; ++i) {
                printf("%02x ", maskedDB[i]);
                if ((i + 1) % 16 == 0)
                    printf("\n");
            }

            printf("\n\n");*/
        }

        template <class InputIterator, class OutputIterator>
        void Decode(InputIterator first, InputIterator last, OutputIterator result) const {}
    };
}

#endif
