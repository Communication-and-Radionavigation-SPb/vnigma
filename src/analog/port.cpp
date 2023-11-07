#include <vnigma/analog/port.h>
#include <cassert>

namespace vnigma { namespace analog {
Port::Port(uint8_t a_index) : Port(Runmode::V10_10R47K, a_index) {}

Port::Port(Runmode a_mode, uint8_t a_index) : index(a_index), mode(a_mode) {
  assert(index < ANALOG_PORTS_COUNT);
}

uint8_t Port::as_hex() const noexcept {
  return mode;
}

Runmode Port::getMode() const noexcept {
  return mode;
}

}}  // namespace vnigma::analog
