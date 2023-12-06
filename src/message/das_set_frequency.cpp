#include <vnigma/message/das_set_frequency.h>
#include <vnigma/util/buffer_manip.h>
#include <vnigma/util/number.hpp>

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
      period(core::never) {
  // trim terminating characters
  buf = trim_buffer(buf);

  // find bound of frequency value
  buffer::size_type lpos = buf.rfind(',');

  if (lpos == buffer::npos) {
    throw make_error(errc::bad_message, "frequency value not found");
  }

  // create value vuffer
  auto valbuf = buf.substr(lpos + 1);

  // validate field
  if (!util::isInteger(valbuf)) {
    throw make_error(errc::bad_message,
                     "vnigma::das::set_frequency invalid frequency value");
  }

  // parse value
  int frequency = util::toInteger(valbuf);
  switch (frequency) {
    case -1:
      period = sendp::never;
      break;
    case 500:
      period = sendp::fast;
      break;
    case 1000:
      period = sendp::normal;
      break;
    case 2000:
      period = sendp::slow;
      break;
    case 5000:
      period = sendp::slower;
      break;
    case 10000:
      period = sendp::slowest;
      break;
    default:
      throw make_error(
          errc::bad_message,
          "vnigma::das::set_frequency unsupported frequency value");
      break;
  }
}

std::string set_frequency::payload() const {
  return std::to_string(period);
}

buffer set_frequency::as_buffer() const {
  return core::control_message<set_frequency>::as_buffer(*this);
}

}}  // namespace vnigma::das
