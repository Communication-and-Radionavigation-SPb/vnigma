#include <vnigma/message/das_get_config.h>

namespace vnigma { namespace das {

get_config::get_config(uuid id, device dev)
    : core::control_message<get_config>(dev), base(id) {}

get_config::get_config(buffer buf)
    : core::control_message<get_config>(buf), base(buf) {}

buffer get_config::as_buffer() {
  return core::control_message<get_config>::as_buffer(*this);
}
}}  // namespace vnigma::das
