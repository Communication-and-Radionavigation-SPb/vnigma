#include <vnigma/message/das_set_config.h>
#include <iostream>
#include <sstream>
#include <vnigma/buffer.hpp>
#include <vnigma/util/number.hpp>

namespace vnigma { namespace das {
set_config::set_config(uuid id, device dev, port_t port)
    : core::control_message<set_config>(dev), base(id), p_(force_move(port)) {}

set_config::set_config(uuid id, device dev, uint8_t index)
    : core::control_message<set_config>(dev), base(id), p_(index) {}

set_config::set_config(uuid id, device dev, uint8_t index,
                       uint8_t configuration)

    : core::control_message<set_config>(dev),
      base(id),
      p_(index, configuration) {}

set_config::set_config(buffer buf)
    : core::control_message<set_config>(buf), base(buf), p_(0, 0xfc) {
  // declare bounding variables
  buffer::size_type rpos, lpos = rpos = 0;
  // find first comma which represents
  // left bound of port identifier
  lpos = buf.find_first_of(',');
  // right bound of port identifier
  rpos = buf.find_first_of(',', lpos + 1);

  // get port index
  uint8_t index = util::toInteger(buf.substr(lpos + 1, rpos - lpos - 1));
  lpos = rpos;
  // get port config
  uint8_t configuration = util::toInteger(buf.substr(lpos + 1, buf.size()), 16);

  p_ = port_t(index, configuration);
}

void set_config::set_port(port_t p) {
  p_ = p;
}

std::string set_config::payload() const {
  std::stringstream ss;
  ss << std::hex << std::uppercase << (unsigned)p_.configuration;
  return ss.str();
}

std::string set_config::port() const {
  return std::to_string(p_.index);
}

buffer set_config::as_buffer() {
  return core::control_message<set_config>::as_buffer(*this);
}
}}  // namespace vnigma::das
