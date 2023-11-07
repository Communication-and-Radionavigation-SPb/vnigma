#include <vnigma/serial/serial_port.h>

namespace vnigma { namespace serial {
/* -------------------------------------------------------------------------- */
Port::Port(uint8_t a_index)
    : is_enabled(false), baudrate(Baud::Baud4800), iface(Iface::RS232), index(a_index) {}
/* -------------------------------------------------------------------------- */
Port::Port(bool a_enabled, Baud a_baudrate, Iface a_iface, uint8_t a_index)
    : is_enabled(a_enabled), baudrate(a_baudrate), iface(a_iface), index(a_index) {}
/* -------------------------------------------------------------------------- */
uint8_t Port::as_hex() const noexcept {
  return 0xE0 | is_enabled | (baudrate << 1) | (iface << 4);
}
/* -------------------------------------------------------------------------- */
}}  // namespace vnigma::serial
