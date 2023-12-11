#include <vnigma/message/das_handshake.h>
#include <vnigma/util/buffer_manip.h>

namespace vnigma { namespace das {

handshake::handshake(serial_number_t sn)
    : core::control_message<handshake>(device(0, Type::general)),
      sn_(std::move(sn)) {}

handshake::handshake(buffer buf)
    : core::control_message<handshake>(buf), sn_("") {
  buf = trim_buffer(buf);

  buf = skip(buf, 1);
  sn_ = std::string(buf.begin(), buf.end());
}

handshake::serial_number_t handshake::serial_number() const {
  return sn_;
}

std::string handshake::payload() const {
  return sn_;
}

buffer handshake::as_buffer() const {
  return core::control_message<handshake>::as_buffer(*this);
}

}}  // namespace vnigma::das
