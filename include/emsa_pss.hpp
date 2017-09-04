#ifndef EMSA_PSS_H
#define EMSA_PSS_H

#include <mgf.hpp>
#include <sha1.hpp>
#include <functional>

namespace cry {

    template <class HashType, class MGFType = MGF1<SHA1>, size_t sLen = HashType::size>
    class emsa_pss{

      public:
        template <class InputIterator, class OutputIterator>
        static OutputIterator encode(InputIterator first, InputIterator last, OutputIterator result, size_t emBits, const std::vector<uint8_t>& saltVal=std::vector<uint8_t>()) {

			size_t  k = emBits / 8;
			//size_t emLen = (emBits % 8) == 0 ? k - 1 : k;
			size_t emLen = (emBits % 8) == 0 ? k : k + 1;
            size_t hLen = HashType::size;
            size_t zBits = 8 * emLen - emBits;

			///////////////////////////////////////////////////////////
			// 2. Let mHash = Hash(M), an octet string of length hLen.
			HashType hash;

			std::vector<uint8_t> mHash(hLen);
			hash(first, last, mHash.begin());

			////////////////////////////////////////////////////////////////////
            // 3.  If emLen < hLen + sLen + 2, output "encoding error" and stop.
            if (emLen < hLen + sLen + 2) {
                throw std::runtime_error("encoding error");
            }
		
			///////////////////////////////////////////////////////////
            // 4.  Generate a random octet string salt of length sLen; 
        	// if sLen = 0, then salt is the empty string.
			std::vector<uint8_t> salt = saltVal;
			if (salt.empty()) {

				salt.resize(sLen);
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> uid(1, 255);

				std::generate(std::begin(salt), std::end(salt), [&uid, &gen]() { return uid(gen); });
			}

			//////////////////////////////////////////////////////////////
            // 5.  Let M' = (0x)00 00 00 00 00 00 00 00 || mHash || salt; 
        	// M' is an octet string of length 8 + hLen + sLen with eight initial zero octets.
            std::vector<uint8_t> M_(8 + hLen + sLen, 0x00);
            std::vector<uint8_t>::iterator it(M_.begin());

            it += 8;

            it = std::copy(mHash.begin(), mHash.end(), it);

			it = std::copy(salt.begin(), salt.end(), it);

			///////////////////////////////////////////////////////
            // 6. Let H = Hash(M'), an octet string of length hLen.
            std::vector<uint8_t> H(hLen);
            hash(M_.begin(), M_.end(), H.begin());

			//////////////////////////////////////////////////////////////////////////////////////
            // 7.  Generate an octet string PS consisting of emLen - sLen - hLen - 2 zero octets.  
        	// The length of PS may be 0.
            size_t psLen = emLen - sLen - hLen - 2;
            std::vector<uint8_t> PS(psLen);

			////////////////////////////////////
            // 8.  Let DB = PS || 0x01 || salt; 
        	// DB is an octet string of length emLen - hLen - 1.

            size_t dbLen = emLen - hLen - 1;
            std::vector<uint8_t> DB(dbLen);
            std::vector<uint8_t>::iterator itDb(DB.begin());

            itDb = std::copy(PS.begin(), PS.end(), itDb);

            *itDb++ = 0x01;

            itDb = std::copy(salt.begin(), salt.end(), itDb);

			//////////////////////////////////////////
            // 9. Let dbMask = MGF (emLen - hLen - 1)
            std::vector<uint8_t> dbMask(dbLen);

            MGFType mgf;
            mgf(H.begin(), H.end(), dbMask.begin(), dbLen);

			/////////////////////////////////////
            // 10. Let maskedDB = DB \xor dbMask.
            std::vector<uint8_t> maskedDB(dbLen);
			std::transform(DB.begin(), DB.end(), dbMask.begin(), maskedDB.begin(), std::bit_xor<>());

			///////////////////////////////////////////////////////////////////////////////////////
            // 11. Set the leftmost 8emLen - emBits bits of the leftmost octet in maskedDB to zero.
            if (zBits > 0)
                *maskedDB.begin() &= (0xFF >> (/*8 - */zBits));

			///////////////////////////////////////
            // 12. Let EM = maskedDB || H || 0xbc.
            result = std::copy(maskedDB.begin(), maskedDB.end(), result);

            result = std::copy(H.begin(), H.end(), result);

            *result++ = 0xbc;

			return result;
        }

        template <class InputIterator, class MInputIterator>
        bool static verify(MInputIterator m_first, MInputIterator m_last, InputIterator em_first, InputIterator em_last, size_t emBits) {

			size_t  k = emBits / 8;
			//size_t emLen = (emBits % 8) == 0 ? k - 1 : k;
			size_t emLen = (emBits % 8) == 0 ? k : k + 1;
			size_t hLen = HashType::size;
			size_t zBits = 8 * emLen - emBits;

			//////////////////////////////////////////////////////////
            // 2. Let mHash = Hash(M), an octet string of length hLen.
            HashType hash;

            std::vector<uint8_t> mHash(hLen);
            hash(m_first, m_last, mHash.begin());

			//////////////////////////////////////////////////////////////////
            // 3.  If emLen < hLen + sLen + 2, output "inconsistent" and stop.
            if (emLen < hLen + sLen + 2) {
                throw std::runtime_error("inconsistent");
            }

			/////////////////////////////////////////////////////////////////////////
            // 4.  If the rightmost octet of EM does not have hexadecimal value 0xbc, 
        	// output "inconsistent" and stop.
            if (*(em_last - 1) != 0xbc) {
				throw std::runtime_error("inconsistent");
            }

			///////////////////////////////////////////////////////////////////
            // 5.  Let maskedDB be the leftmost emLen - hLen - 1 octets of EM, 
        	// and let H be the next hLen octets.
            size_t dbLen = emLen - hLen - 1;
            std::vector<uint8_t> maskedDB(em_first, em_first + dbLen);

            std::vector<uint8_t> H(em_first + dbLen, em_first + dbLen + hLen);

			///////////////////////////////////////////////////////////////////////////////////////////////////////
            // 6. If the leftmost 8emLen - emBits bits of the leftmost octet in maskedDB are not all equal to zero, 
        	// output "inconsistent" and stop.
            if (zBits > 0) 
				if (*maskedDB.begin() >> zBits == (0xFF /*<<*/>> zBits)) {
					throw std::runtime_error("inconsistent");
            }

			/////////////////////////////////////////////
            // 7. Let dbMask = MGF(H, emLen - hLen - 1).
            std::vector<uint8_t> dbMask(dbLen);

            MGFType mgf;
            mgf(H.begin(), H.end(), dbMask.begin(), dbLen);

			/////////////////////////////////////
            // 8. Let DB = maskedDB \xor dbMask.
            std::vector<uint8_t> DB(dbLen);
			std::transform(maskedDB.begin(), maskedDB.end(), dbMask.begin(), DB.begin(), std::bit_xor<>());

			////////////////////////////////////////////////////////////////////////////////
            // 9. Set the leftmost 8emLen - emBits bits of the leftmost octet in DB to zero.
            if (zBits > 0)
                *DB.begin() &= (0xFF >> (/*8 - */zBits));

			/////////////////////////////////////////////////////////////////////////
            // 10. If the emLen - hLen - sLen - 2 leftmost octets of DB are not zero
            // or if the octet at position emLen - hLen - sLen - 1 (the leftmost
            // position is "position 1") does not have hexadecimal value 0x01,
            // output "inconsistent" and stop.
            size_t i = 0;
            for (; DB[i] == 0x00 && i < dbLen - 1; ++i)
                ;

            if (DB[i] != 0x01) {
				throw std::runtime_error("inconsistent");
            }

			//////////////////////////////////////////////////
            // 11. Let salt be the em_last sLen octets of DB.
            auto sIt = DB.end();
            std::advance(sIt, -sLen);

            std::vector<uint8_t> salt(sIt, DB.end());

			////////////////////////////////////////////////////////////////
            // 12. Let  M' = (0x)00 00 00 00 00 00 00 00 || mHash || salt ;
            // M' is an octet string of length 8 + hLen + sLen with eight initial zero octets.
            auto M_ = std::vector<uint8_t>(8 + hLen + sLen);
            auto it = M_.begin();
            it += 8;

            it = std::copy(mHash.begin(), mHash.end(), it);

            it = std::copy(salt.begin(), salt.end(), it);

            auto H_ = std::vector<uint8_t>(hLen);
            hash(M_.begin(), M_.end(), H_.begin());

            return (H == H_);
        }
    };
}

#endif
