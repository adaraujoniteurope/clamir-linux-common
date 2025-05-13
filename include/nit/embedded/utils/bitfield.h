#ifndef _utils_bitfield_h_
#define _utils_bitfield_h_

#include <cstddef>
#include <cstdint>

namespace nit::embedded::utils {
template <typename type, size_t msb, size_t lsb> struct bitfield {
public:
  static const type mask =
      (type)(((uint64_t)((uint64_t)1 << ((uint64_t)msb + 1)) - (uint64_t)1) &
             ~(uint64_t)(((uint64_t)1 << ((uint64_t)lsb)) - (uint64_t)1));

  static type get(type v) { return (v & mask) >> lsb; }

  static type set(type v, type base = 0) {
    base &= ~mask;
    base |= (v << lsb) & mask;
    return base;
  }

  bitfield() = delete;
  bitfield(bitfield &) = delete;
  bitfield(bitfield &&) = delete;
};
} // namespace nit::embedded::utils

#endif