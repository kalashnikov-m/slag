#ifndef PKCS1_V1_5_HPP
#define PKCS1_V1_5_HPP

#include <random>
#include <algorithm>

namespace cry {

    class eme_pkcs1 {

      public:
        template <class InputIterator, class OutputIterator>
        static OutputIterator encode(InputIterator first, InputIterator last, OutputIterator result, size_t k) {

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

            result = std::copy(first, last, result);

			return result;
        }

        template <class InputIterator, class OutputIterator>
        static OutputIterator decode(InputIterator first, InputIterator last, OutputIterator result, size_t k) {
            if (first != last && *first++ != 0x00)
                throw std::exception("decryption error");

            if (first != last && *first++ != 0x02)
				throw std::exception("decryption error");

            for (; first != last && *first != 0x00; ++first)
                ;

            if (*first != 0x00)
				throw std::exception("decryption error");

            result = std::copy(first, last, result);

			return result;
        }
    };	
}

#endif
