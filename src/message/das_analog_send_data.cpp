#include <vnigma/message/das_analog_send_data.h>
#include <vnigma/util/buffer_manip.h>

#include <stdexcept>

namespace vnigma { namespace das {

analog_send_data::analog_send_data(device dev, data_variant data)
    : core::control_message<analog_send_data>(dev),
      send_data(std::move(data)) {}

analog_send_data::analog_send_data(buffer buf)
    : core::control_message<analog_send_data>(buf),
      send_data(std::move(buf), core::analog) {
  if (target_type() != core::analog) {
    throw std::runtime_error(
        "analog_send_data allows only analog device target");
  }
}

buffer analog_send_data::as_buffer() const {
  return core::control_message<analog_send_data>::as_buffer(*this);
}

std::string analog_send_data::payload() const {
  return send_data::payload();
}

device analog_send_data::get_device() const {
  return core::control_message<analog_send_data>::get_device().value();
}

analog::data analog_send_data::get_data() const {
  return send_data::get_data().get<analog::data>();
}

}}  // namespace vnigma::das
