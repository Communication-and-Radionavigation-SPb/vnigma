#include <vnigma/ports/serial_port.h>
#include <vnigma/util/binary.hpp>

namespace vnigma { namespace serial {

port::port(uint8_t value) : port(value, binary<11110111>::value) {}

port::port(uint8_t idx, uint8_t config) : index(idx), configuration(config) {}

bool port::is_enabled() const noexcept {
  return configuration & 0x1;
}

BaudRate port::baud_rate() const noexcept {
  static const unsigned baud_mask = binary<1110>::value;
  auto part = (configuration & baud_mask) >> 1;
  return (BaudRate)part;
}

void port::set_baud_rate(BaudRate brt) noexcept {
  static const unsigned permanent = binary<1111>::value << 4;
  configuration = permanent | brt << 1 | is_enabled();
}

void port::enable() noexcept {
  configuration = configuration | 0x1;
}

void port::disable() noexcept {
  static const uint8_t disable_mask = binary<11111110>::value;
  configuration = configuration & disable_mask;
}

}}  // namespace vnigma::serial
