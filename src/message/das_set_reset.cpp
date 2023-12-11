#include <vnigma/message/das_set_reset.h>
#include <vnigma/exception.hpp>

namespace vnigma { namespace das {

set_reset::set_reset(uuid id, device dev)
    : core::control_message<set_reset>(dev), base(id) {}

set_reset::set_reset(buffer buf)
    : core::control_message<set_reset>(buf), base(buf) {}

buffer set_reset::as_buffer() const {
  return core::control_message<set_reset>::as_buffer(*this);
}

device set_reset::get_device() const {
  return core::control_message<set_reset>::get_device().value();
}

}}  // namespace vnigma::das
