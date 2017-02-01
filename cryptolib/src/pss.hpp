#ifndef PSS_H
#define PSS_H

#include <mgf.hpp>
#include <iterator>

std::function<void(const std::vector<uint8_t>&)> trace = [](const std::vector<uint8_t>& container) {
    for (size_t i = 0; i < container.size(); ++i) {
        printf("%02x ", container[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }

    printf("\n\n");
};

namespace cry {

    template <class HashType, class MGFType = MGF1<SHA1>, size_t sLen = HashType::size>
    class PSS {

      public:
        template <class InputIterator, class OutputIterator>
        void Encode(InputIterator first, InputIterator last, OutputIterator result, size_t emBits) const {

            size_t emLen = emBits / 8;
            size_t hLen = HashType::size;
            size_t zBits = 8 * emLen - emBits;

            // 3.  If emLen < hLen + sLen + 2, output "encoding error" and stop.
            if (emLen < hLen + sLen + 2) {
                throw 2; // encoding error
            }

            // 2. Let mHash = Hash(M), an octet string of length hLen.
            HashType hash;

            std::vector<uint8_t> mHash(hLen);
            hash(first, last, mHash.begin());

            // 4.  Generate a random octet string salt of length sLen; if sLen = 0, then salt is the empty string.
            // std::vector<uint8_t> salt = {0xe3, 0xb5, 0xd5, 0xd0, 0x02, 0xc1, 0xbc, 0xe5, 0x0c, 0x2b, 0x65, 0xef, 0x88, 0xa1, 0x88, 0xd8, 0x3b, 0xce, 0x7e, 0x61};
            std::vector<uint8_t> salt(sLen);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> uid(1, 255);

            std::generate(std::begin(salt), std::end(salt), [&uid, &gen]() { return uid(gen); });

            // 5.  Let M' = (0x)00 00 00 00 00 00 00 00 || mHash || salt; M' is an octet string of length 8 + hLen + sLen with eight initial zero octets.
            std::vector<uint8_t> M_(8 + hLen + sLen, 0x00);
            std::vector<uint8_t>::iterator it(M_.begin());

            it += 8;

            std::copy(mHash.begin(), mHash.end(), it);
            it += hLen;

            std::copy(salt.begin(), salt.end(), it);
            it += sLen;

            // 6. Let H = Hash(M'), an octet string of length hLen.
            std::vector<uint8_t> H(hLen);
            hash(M_.begin(), M_.end(), H.begin());

            // 7.  Generate an octet string PS consisting of emLen - sLen - hLen - 2 zero octets.  The length of PS may be 0.
            size_t psLen = emLen - sLen - hLen - 2;
            std::vector<uint8_t> PS(psLen);

            //  8.  Let DB = PS || 0x01 || salt; DB is an octet string of length emLen - hLen - 1.
            size_t dbLen = emLen - hLen - 1;
            std::vector<uint8_t> DB(dbLen);
            std::vector<uint8_t>::iterator itDb(DB.begin());

            std::copy(PS.begin(), PS.end(), itDb);
            itDb += psLen;

            *itDb++ = 0x01;

            std::copy(salt.begin(), salt.end(), itDb);
            itDb += sLen;

            // 9. Let dbMask = MGF (emLen - hLen - 1)
            std::vector<uint8_t> dbMask(dbLen);

            MGFType mgf;
            mgf(H.begin(), H.end(), dbMask.begin(), dbLen);

            // 10. Let maskedDB = DB \xor dbMask.
            std::vector<uint8_t> maskedDB(dbLen);
            for (size_t i = 0; i < dbLen; ++i) {
                maskedDB[i] = DB[i] ^ dbMask[i];
            }

            // 11. Set the leftmost 8emLen - emBits bits of the leftmost octet in maskedDB to zero.
            if (zBits > 0)
                *maskedDB.begin() &= (0xFF >> (8 - zBits));

            // 12. Let EM = maskedDB || H || 0xbc.
            std::copy(maskedDB.begin(), maskedDB.end(), result);
            result += dbLen;

            std::copy(H.begin(), H.end(), result);
            result += hLen;

            *result++ = 0xbc;
        }

        template <class InputIterator, class MInputIterator>
        bool Verify(MInputIterator mFirst, MInputIterator mLast, InputIterator first, InputIterator last, size_t emBits) const {

            size_t emLen = emBits / 8;
            size_t hLen = HashType::size;
            size_t zBits = 8 * emLen - emBits;

            // 2. Let mHash = Hash(M), an octet string of length hLen.
            HashType hash;

            std::vector<uint8_t> mHash(hLen);
            hash(mFirst, mLast, mHash.begin());

            // 3.  If emLen < hLen + sLen + 2, output "inconsistent" and stop.
            if (emLen < hLen + sLen + 2) {
                throw 1; // inconsistent
            }

            // 4.  If the rightmost octet of EM does not have hexadecimal value 0xbc, output "inconsistent" and stop.
            // printf("%02x\n", *(--last));
            if (*(last - 1) != 0xbc) {
                throw 1; // inconsistent;
            }

            //  5.  Let maskedDB be the leftmost emLen - hLen - 1 octets of EM, and let H be the next hLen octets.
            size_t dbLen = emLen - hLen - 1;
            std::vector<uint8_t> maskedDB(first, first + dbLen);

            std::vector<uint8_t> H(first + dbLen, first + dbLen + hLen);

            // 6. If the leftmost 8emLen - emBits bits of the leftmost octet in maskedDB are not all equal to zero, output "inconsistent" and stop.
            if (zBits > 0 && *maskedDB.begin() & (0xFF << zBits)) {
                throw 1; // inconsistent
            }

            // 7. Let dbMask = MGF(H, emLen - hLen - 1).
            std::vector<uint8_t> dbMask(dbLen);

            MGFType mgf;
            mgf(H.begin(), H.end(), dbMask.begin(), dbLen);

            // 8. Let DB = maskedDB \xor dbMask.
            std::vector<uint8_t> DB(dbLen);
            for (size_t i = 0; i < dbLen; ++i) {
                DB[i] = maskedDB[i] ^ dbMask[i];
            }

            // 9. Set the leftmost 8emLen - emBits bits of the leftmost octet in DB to zero.
            if (zBits > 0)
                *DB.begin() &= (0xFF >> (8 - zBits));

            // 10. If the emLen - hLen - sLen - 2 leftmost octets of DB are not zero
            // or if the octet at position emLen - hLen - sLen - 1 (the leftmost
            // position is "position 1") does not have hexadecimal value 0x01,
            // output "inconsistent" and stop.
            size_t i = 0;
            for (; DB[i] == 0x00 && i < dbLen - 1; ++i)
                ;
            if (DB[i] != 0x01) {
                throw 1; // inconsistent
            }

            // 11. Let salt be the last sLen octets of DB.
            auto sIt = DB.end();
            std::advance(sIt, -sLen);

            std::vector<uint8_t> salt(sIt, DB.end());

            // 12. Let  M' = (0x)00 00 00 00 00 00 00 00 || mHash || salt ;
            // M' is an octet string of length 8 + hLen + sLen with eight initial zero octets.
            auto M_ = std::vector<uint8_t>(8 + hLen + sLen);
            auto it = M_.begin();
            it += 8;

            std::copy(mHash.begin(), mHash.end(), it);
            it += hLen;

            std::copy(salt.begin(), salt.end(), it);
            it += sLen;

            auto H_ = std::vector<uint8_t>(hLen);
            hash(M_.begin(), M_.end(), H_.begin());

            return (H == H_);
        }
    };
}

#endif
