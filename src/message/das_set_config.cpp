#include <vnigma/message/das_set_config.h>
#include <sstream>

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
}}  // namespace vnigma::das
