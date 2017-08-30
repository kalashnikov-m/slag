//
// Created by max on 07.07.17.
//

#ifndef RSAES_PKCS1_H
#define RSAES_PKCS1_H

#include "basic_int.hpp"

namespace cry {

template <class Encoder, class T = bigint8_t>
struct rsaes_pkcs1 {
	template <class InputIterator, class OutputIterator>
	static OutputIterator encrypt(InputIterator first, InputIterator last, OutputIterator result, const T &e, const T &n, size_t modBits) {

    /*auto encoded = Encoder::encode(first, last, modulusBits / 8);

    T arg(encoded.begin(), encoded.end());
    T result = cry::pow_mod(arg, privateExponent, modulus);

    signature = (std::vector<uint8_t>)result;*/

		return result;
  }

	template <class InputIterator, class OutputIterator>
	static bool decrypt(InputIterator first, InputIterator last, OutputIterator result, const T &d, const T &n, size_t modBits) {
	  

		return result;
  }
};

} // namespace cry

#endif // CRY_RSA_H
