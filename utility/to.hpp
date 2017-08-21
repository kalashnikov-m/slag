#ifndef TO_HPP
#define TO_HPP

#include "basic_int.hpp"
#include <vector>

namespace cry {

template <class T, size_t sz = sizeof(T)> struct to_bvector {
  constexpr const std::vector<uint8_t> operator()(T t) const noexcept {
    return std::vector<uint8_t>();
  }
};

template <class T> struct to_bvector<T, 2> {
  constexpr const std::vector<uint8_t> operator()(T t) const noexcept {
    return {static_cast<uint8_t>((t & 0xff00) >> 8),
            static_cast<uint8_t>((t & 0x00ff))};
  }
};

template <class T> struct to_bvector<T, 4> {
  constexpr const std::vector<uint8_t> operator()(T t) const noexcept {
    return {static_cast<uint8_t>((t & 0xff000000) >> 24),
            static_cast<uint8_t>((t & 0x00ff0000) >> 16),
            static_cast<uint8_t>((t & 0x0000ff00) >> 8),
            static_cast<uint8_t>((t & 0x000000ff))};
  }
};

template <class T> struct to_bvector<T, 8> {
  constexpr const std::vector<uint8_t> operator()(T t) const noexcept {
    return {static_cast<uint8_t>((t & 0xff00000000000000) >> 56),
            static_cast<uint8_t>((t & 0x00ff000000000000) >> 48),
            static_cast<uint8_t>((t & 0x0000ff0000000000) >> 40),
            static_cast<uint8_t>((t & 0x000000ff00000000) >> 32),
            static_cast<uint8_t>((t & 0x00000000ff000000) >> 24),
            static_cast<uint8_t>((t & 0x0000000000ff0000) >> 16),
            static_cast<uint8_t>((t & 0x000000000000ff00) >> 8),
            static_cast<uint8_t>((t & 0x00000000000000ff))};
  }
};

template <> struct to_bvector<cry::bigint8_t> {
  const std::vector<uint8_t> operator()(cry::bigint8_t t) const noexcept {

    std::vector<uint8_t> result(t);

    return result;
  }
};
} // namespace cry

#endif
