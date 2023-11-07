#pragma once
#include <vnigma/export.h>
#include <vnigma/serial/types.h>
#include <cstdint>

namespace vnigma { namespace serial {
class VNIGMA_EXPORT Port {
 public:
  uint8_t index;
  bool is_enabled;
  Baud baudrate;
  Iface iface;

 public:
  Port(uint8_t a_index = 0);
  Port(bool a_enabled, Baud a_baudrate, Iface a_iface, uint8_t a_index = 0);

 public:
  uint8_t as_hex() const noexcept;
};

struct portSort {
  bool inline operator()(const Port& left, const Port& right) const {
    return left.index < right.index;
  }
};

}}  // namespace vnigma::serial
