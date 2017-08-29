#ifndef PKCS1_V1_5_HPP
#define PKCS1_V1_5_HPP

#include <algorithm>
#include "oid.hpp"

namespace cry {
    
	template<class DigestType=SHA1>
    class emsa_pkcs1 {

      public:
        template <class InputIterator>
        static std::vector<uint8_t> encode(InputIterator first, InputIterator last, size_t emLen) {

			std::vector<uint8_t> out(emLen);
			auto result = out.begin();

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

			return out;
        }

        template <class InputIterator>
        static std::vector<uint8_t> decode(InputIterator first, InputIterator last, size_t emLen) {

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
					throw std::exception("signature invalid");
			}
            
            if (first != last && *first++ != 0x01)
				throw std::exception("signature invalid");

            for (; first != last && *first != 0x00 && *first == 0xff; ++first)
                ;

            if (*first++ != 0x00)
				throw std::exception("signature invalid");

            auto oid = OID<DigestType>::value();

			size_t oidLen = oid.size();
            bool eq = std::equal(std::begin(oid), std::end(oid), first);
            if (!eq) {
				throw std::exception("signature invalid");
            }

            first += oidLen;

			return std::vector<uint8_t>(first, last);
        }

      private:
    };
}

#endif
