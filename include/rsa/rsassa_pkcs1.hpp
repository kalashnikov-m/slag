//
// Created by max on 07.07.17.
//

#ifndef RSA_SIGNVERIFY_H
#define RSA_SIGNVERIFY_H

#include "basic_int.hpp"

namespace cry {

template <class Encoder, class IntType = bigint8_t>
struct rsassa_pkcs1 {
  template <size_t modulusBits, class InputIterator>
  static void sign(InputIterator first, InputIterator last, const IntType &privateExponent, const IntType &modulus, std::vector<uint8_t> &signature) {

    auto encoded = Encoder::encode(first, last, modulusBits / 8);

    IntType arg(encoded.begin(), encoded.end());
    IntType result = cry::pow_mod(arg, privateExponent, modulus);

    signature = (std::vector<uint8_t>)result;
  }

  template <size_t modulusBits, class InputIterator>
  static bool verify(InputIterator s_first, InputIterator s_last, InputIterator m_first, InputIterator m_last, const IntType &publicExponent, const IntType &modulus) {

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
    IntType s(s_first, s_last);

    ///////////////////////////////////////////////
    // 2b. Apply the RSAVP1 verification primitive
    IntType m = cry::pow_mod(s, publicExponent, modulus);

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

    return f;
  }
};

} // namespace cry

#endif // CRY_RSA_H
