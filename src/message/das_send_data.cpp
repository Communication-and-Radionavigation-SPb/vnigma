#include <vnigma/message/das_send_data.h>

namespace vnigma { namespace das {
send_data::send_data(uuid id, device dev)
    : core::control_message<send_data>(dev), base(id) {}

send_data::send_data(buffer buf)
    : core::control_message<send_data>(buf), base(buf) {}

buffer send_data::as_buffer() {
  return core::control_message<send_data>::as_buffer(*this);
}

std::string send_data::payload() {
  return "";
}

}}  // namespace vnigma::das
