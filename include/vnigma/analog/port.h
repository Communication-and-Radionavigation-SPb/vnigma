#pragma once

#include <vnigma/analog/types.h>
#include <vnigma/export.h>
#include <cstdint>

namespace vnigma { namespace analog {
class VNIGMA_EXPORT Port {
 public:
  uint8_t index;

 private:
  Runmode mode;

 public:
  Port(uint8_t a_index = 0);
  Port(Runmode a_mode, uint8_t a_index = 0);

 public:
  uint8_t as_hex() const noexcept;

 public:
  Runmode getMode() const noexcept;
};

struct portSort {
  bool inline operator()(const Port& left, const Port& right) const {
    return left.index < right.index;
  }
};
}}  // namespace vnigma::analog
