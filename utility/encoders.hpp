#ifndef TO_HPP
#define TO_HPP

#include "basic_int.hpp"

namespace cry {

template <class T> struct Endian {
protected:
  static T swap(const T &src) noexcept {
    T dst;

    switch (sizeof(T)) {
    case 8:
      static_cast<uint8_t *>(&dst)[0] = static_cast<uint8_t *>(&src)[7];
      static_cast<uint8_t *>(&dst)[1] = static_cast<uint8_t *>(&src)[6];
      static_cast<uint8_t *>(&dst)[2] = static_cast<uint8_t *>(&src)[5];
      static_cast<uint8_t *>(&dst)[3] = static_cast<uint8_t *>(&src)[4];
      static_cast<uint8_t *>(&dst)[4] = static_cast<uint8_t *>(&src)[3];
      static_cast<uint8_t *>(&dst)[5] = static_cast<uint8_t *>(&src)[2];
      static_cast<uint8_t *>(&dst)[6] = static_cast<uint8_t *>(&src)[1];
      static_cast<uint8_t *>(&dst)[7] = static_cast<uint8_t *>(&src)[0];
      break;
    case 4:
      static_cast<uint8_t *>(&dst)[0] = static_cast<uint8_t *>(&src)[3];
      static_cast<uint8_t *>(&dst)[1] = static_cast<uint8_t *>(&src)[2];
      static_cast<uint8_t *>(&dst)[2] = static_cast<uint8_t *>(&src)[1];
      static_cast<uint8_t *>(&dst)[3] = static_cast<uint8_t *>(&src)[0];
      break;

    case 2:
      static_cast<uint8_t *>(&dst)[0] = static_cast<uint8_t *>(&src)[1];
      static_cast<uint8_t *>(&dst)[1] = static_cast<uint8_t *>(&src)[1];
      break;

    default:
      break;
    };
  }
};

} // namespace cry

#endif
