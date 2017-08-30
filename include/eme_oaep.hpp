#ifndef EME_OAEP_H
#define EME_OAEP_H

#include <iterator>

#include <mgf.hpp>

#include "sha1.hpp"

namespace cry {

    template <class HashType, class MGFType = MGF1<SHA1>, size_t hLen = HashType::size>
    class eme_oaep {

    public:
		using hash_type = HashType;

      public:
        template <class InputIterator, class OutputIterator>
        static OutputIterator encode(InputIterator first, InputIterator last, OutputIterator result, size_t k, const std::vector<uint8_t>& seedVal = std::vector<uint8_t>(), const std::vector<uint8_t>& L = std::vector<uint8_t>()) {

            auto mLen = std::distance(first, last);

			////////////////////////////////////////////////////////
            // a. Let lHash = Hash (L), an octet string of length hLen
            auto lHash = std::vector<uint8_t>(hLen);

            HashType hash;
            hash(L.begin(), L.end(), lHash.begin());

			///////////////////////////////////////////////////////////////////////////////
            // b. Generate an octet string PS consisting of k – mLen – 2hLen – 2 zero octets.
            // The length of PS may be zero.
            size_t psLen = k - mLen - 2 * hLen - 2;
            auto PS = std::vector<uint8_t>(psLen);

			/////////////////////////////////////////////////////////////////////////////
            // c. Concatenate lHash, PS, a single octet with hexadecimal value 0x01, and
            // the message M to form a data block DB of length k – hLen – 1 octets as
            // DB = lHash || PS || 0x01 || M .
            size_t dbLen = k - hLen - 1;
            auto DB = std::vector<uint8_t>(dbLen);
            auto it = DB.begin();

            std::copy(lHash.begin(), lHash.end(), it);
            it += hLen;

            std::copy(PS.begin(), PS.end(), it);
            it += psLen;

            *it++ = 0x01;

            std::copy(first, last, it);
            it += mLen;

			/////////////////////////////////////////////////////////
			// d. Generate a random octet string seed of length hLen.
            auto seed = seedVal;
            if (seed.empty()) {
			  seed.reserve(hLen);

              std::random_device rd;
              std::mt19937 gen(rd());
              std::uniform_int_distribution<> uid(1, 255);

              std::generate(std::begin(seed), std::end(seed), [&uid, &gen]() { return uid(gen); });
            }

			////////////////////////////////////////////
            // e. Let dbMask = MGF (seed, k – hLen – 1)
            auto dbMask = std::vector<uint8_t>(dbLen);

            MGFType mgf;
            mgf(seed.begin(), seed.end(), dbMask.begin(), dbLen);

			/////////////////////////////////////
            // f. Let maskedDB = DB \xor dbMask.
            std::vector<uint8_t> maskedDB(dbLen);
            for (size_t i = 0; i < dbLen; ++i) {
                maskedDB[i] = DB[i] ^ dbMask[i];
            }

			/////////////////////////////////////////
            // g. Let seedMask = MGF(maskedDB, hLen).
            std::vector<uint8_t> seedMask(hLen);
            mgf(maskedDB.begin(), maskedDB.end(), seedMask.begin(), hLen);

			//////////////////////////////////////////
            // h. Let maskedSeed = seed \xor seedMask.
            std::vector<uint8_t> maskedSeed(hLen);
            for (size_t i = 0; i < hLen; ++i) {
                maskedSeed[i] = seed[i] ^ seedMask[i];
            }

			/////////////////////////////////////////////////////////////////////////////
			// i. Concatenate a single octet with hexadecimal value 0x00, maskedSeed, and
            // maskedDB to form an encoded message EM of length k octets as
            // EM = 0x00 || maskedSeed || maskedDB.

            *result++ = 0x00;

            std::copy(maskedSeed.begin(), maskedSeed.end(), result);
            result += hLen;

            std::copy(maskedDB.begin(), maskedDB.end(), result);
            result += dbLen;

			return result;
        }

        template <class InputIterator, class OutputIterator>
        static OutputIterator decode(InputIterator first, InputIterator last, OutputIterator result, size_t k, const std::vector<uint8_t>& L = std::vector<uint8_t>()) {

          // a.
          auto lHash = std::vector<uint8_t>(hLen);

          HashType hash;
          hash(L.begin(), L.end(), lHash.begin());

          // b. Separate the encoded message EM into a single octet Y, an octet
          // string maskedSeed of length hLen, and an octet string maskedDB of
          // length k - hLen - 1 as EM = Y || maskedSeed || maskedDB.
          size_t sz = std::distance(first, last);

		  if (sz > k)
		  {
			  throw std::exception("decryption error");
		  }

          if (sz == k) {
            if (*first++ != 0x00) {
              throw std::exception("decryption error");
            }
          }

          std::vector<uint8_t> maskedSeed(first, first + hLen);
          first += hLen;

          size_t dbLen = k - hLen - 1;
          std::vector<uint8_t> maskedDB(first, first + dbLen);

          // c. Let seedMask = MGF(maskedDB, hLen).
          std::vector<uint8_t> seedMask(hLen);

          MGFType mgf;
          mgf(maskedDB.begin(), maskedDB.end(), seedMask.begin(), hLen);

          // d. Let seed = maskedSeed \xor seedMask.
          std::vector<uint8_t> seed(hLen);
          for (size_t i = 0; i < hLen; ++i) {
            seed[i] = maskedSeed[i] ^ seedMask[i];
          }

          // e. Let dbMask = MGF(seed, k - hLen - 1).
          std::vector<uint8_t> dbMask(dbLen);
          mgf(seed.begin(), seed.end(), dbMask.begin(), dbLen);

          // f. Let DB = maskedDB \xor dbMask.
          std::vector<uint8_t> DB(dbLen);
          for (size_t i = 0; i < dbLen; ++i) {
            DB[i] = maskedDB[i] ^ dbMask[i];
          }

          // g. Separate DB into an octet string lHash' of length hLen, a
          // (possibly empty) padding string PS consisting of octets with
          // hexadecimal value 0x00, and a message M as DB = lHash' || PS ||
          // 0x01 || M. If there is no octet with hexadecimal value 0x01 to
          // separate PS from M, if lHash does not equal lHash', or if Y is
          // nonzero, output "decryption error" and stop.

          auto it = DB.begin();
          std::vector<uint8_t> lHash_(DB.begin(), it + hLen);
          it += hLen;
          if (lHash != lHash_) {
            throw std::exception("decryption_error");
          }

          for (; it != DB.end() && *it == 0x00; ++it)
            ;

          if (*it++ != 0x01) {
            throw std::exception("decryption_error");
          }

          result = std::copy(it, DB.end(), result);
		  
          return result;
        }
    };
}

#endif
