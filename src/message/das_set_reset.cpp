#include <vnigma/message/das_set_reset.h>
#include <vnigma/exception.hpp>

namespace vnigma { namespace das {

set_reset::set_reset(uuid id, device dev)
    : core::control_message<set_reset>(dev), base(id) {}

set_reset::set_reset(buffer buf)
    : core::control_message<set_reset>(buf), base(buf) {}

buffer set_reset::as_buffer() {
  return core::control_message<set_reset>::as_buffer(*this);
}

}}  // namespace vnigma::das
