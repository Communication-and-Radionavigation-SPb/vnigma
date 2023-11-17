#include <vnigma/message/das_set_frequency.h>

namespace vnigma { namespace das {
set_frequency::set_frequency(uuid id, device dev)
    : core::control_message<set_frequency>(dev),
      base(id),
      period(sendp::normal) {}

set_frequency::set_frequency(uuid id, device dev, sendp a_period)
    : core::control_message<set_frequency>(dev), base(id), period(a_period) {}

set_frequency::set_frequency(buffer buf)
    : core::control_message<set_frequency>(buf),
      base(buf),
      period(core::never) {}

std::string set_frequency::payload() const {
  return std::to_string(period);
}

buffer set_frequency::as_buffer() {
  return core::control_message<set_frequency>::as_buffer(*this);
}

}}  // namespace vnigma::das
