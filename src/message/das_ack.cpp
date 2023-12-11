#include <vnigma/message/das_ack.h>
#include <vnigma/util/buffer_manip.h>

namespace vnigma { namespace das {

ack::ack(uuid id, device dev, buffer payload)
    : core::control_message<ack>(dev), base(id), payload_(std::move(payload)) {}

ack::ack(buffer buf) : core::control_message<ack>(buf), base(buf) {
  buf = trim_buffer(buf);

  buf = skip(buf, 4);
  payload_ = std::move(buf);
}

buffer ack::as_buffer() const {
  return core::control_message<ack>::as_buffer(*this);
}

device ack::get_device() const {
  return core::control_message<ack>::get_device().value();
}

std::string ack::payload() const {
  return std::string{payload_.begin(), payload_.end()};
}

}}  // namespace vnigma::das
