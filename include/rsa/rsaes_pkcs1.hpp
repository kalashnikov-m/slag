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
	static OutputIterator encrypt(InputIterator first, InputIterator last, const T &privateExponent, const T &modulus, size_t modBits) {

    /*auto encoded = Encoder::encode(first, last, modulusBits / 8);

    T arg(encoded.begin(), encoded.end());
    T result = cry::pow_mod(arg, privateExponent, modulus);

    signature = (std::vector<uint8_t>)result;*/
  }

	template <class InputIterator, class OutputIterator>
	static bool decrypt(InputIterator first, InputIterator last, const T &publicExponent, const T &modulus, size_t modBits) {
	  /*
    ///////////////////////
    // 1. Length checking:
    size_t k = modulusBits / 8;
    auto sz = std::distance(s_first, s_last);
    if (k != sz) {
      return false;
    }

    ////////////////////////
    // 2. RSA verification:

    //////////////////////////////////////////////////////////////////////
    // 2a. Convert the signature S to an integer signature representative
    T s(s_first, s_last);

    ///////////////////////////////////////////////
    // 2b. Apply the RSAVP1 verification primitive
    T m = cry::pow_mod(s, publicExponent, modulus);

    ////////////////////////////////////////////////////////////////////////////////////////
    // 2c. Convert the message representative m to an encoded message EM of length k octets
    std::vector<uint8_t> EM(m);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 3. Apply the EMSA-PKCS1-v1_5 encoding operation to the message M to produce a second encoded message EM’ of length k octets:
    auto EM_ = Encoder::encode(m_first, m_last, k);

    ////////////////////////////////////////////////////////////////////////
    // 4. Compare the encoded message EM and the second encoded message EM’
    bool f(false);
    auto it(EM_.begin());

    if (EM_.size() != EM.size()) {
      ++it;
    }

    f = std::equal(it, std::end(EM_), std::begin(EM), std::end(EM));

    return f;*/
  }
};

} // namespace cry

#endif // CRY_RSA_H
