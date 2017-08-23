//
// Created by max on 07.07.17.
//

#ifndef RSA_SIGNVERIFY_H
#define RSA_SIGNVERIFY_H

#include "basic_int.hpp"

namespace cry {

template <class Encoder, class T = bigint8_t>
struct RSASignVerify {
  template <size_t modulusBits, class InputIterator>
  static void sign(InputIterator first, InputIterator last, const T &privateExponent, const T &modulus, std::vector<uint8_t> &signature) {

    auto encoded = Encoder::Encode(first, last, modulusBits / 8);

    T arg(encoded.begin(), encoded.end());
    T result = cry::PowMod(arg, privateExponent, modulus);

    signature = (std::vector<uint8_t>)result;
  }

  template <size_t modulusBits, class InputIterator>
  static bool verify(InputIterator s_first, InputIterator s_last, InputIterator m_first, InputIterator m_last, const T &publicExponent, const T &modulus) {

    T signature(s_first, s_last);
    T t = cry::PowMod(signature, publicExponent, modulus);

    std::vector<uint8_t> encoded(t);

    auto decoded = Encoder::Decode(encoded.begin(), encoded.end(), modulusBits / 8);

    return std::equal(m_first, m_last, decoded.begin(), decoded.end());
  }
};

} // namespace cry

#endif // CRY_RSA_H
