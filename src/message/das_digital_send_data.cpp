#include <vnigma/message/das_digital_send_data.h>

#include <stdexcept>

namespace vnigma { namespace das {

digital_send_data::digital_send_data(device dev, digital::data data)
    : core::control_message<digital_send_data>(dev),
      send_data(std::move(data)) {}

digital_send_data::digital_send_data(buffer buf)
    : core::control_message<digital_send_data>(buf),
      send_data(std::move(buf), core::digital) {
  if (target_type() != core::digital) {
    throw std::runtime_error(
        "digital_send_data allows only digital device target");
  }
}

buffer digital_send_data::as_buffer() const {
  return core::control_message<digital_send_data>::as_buffer(*this);
}

std::string digital_send_data::payload() const {
  return send_data::payload();
}

device digital_send_data::get_device() const {
  return core::control_message<digital_send_data>::get_device().value();
}

digital::data digital_send_data::get_data() const {
  return send_data::get_data().get<digital::data>();
}

}}  // namespace vnigma::das
