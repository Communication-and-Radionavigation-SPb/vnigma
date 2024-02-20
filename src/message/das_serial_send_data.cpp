#include <vnigma/message/das_serial_send_data.h>
#include <vnigma/util/buffer_manip.h>
#include <vnigma/util/number.hpp>

#include <stdexcept>

namespace vnigma { namespace das {

serial_send_data::serial_send_data(uint8_t index, device dev, data_variant data)
    : core::control_message<serial_send_data>(dev),
      send_data(std::move(data)),
      port_index_(index) {}

serial_send_data::serial_send_data(buffer income)
    : core::control_message<serial_send_data>(income),
      send_data(income, core::serial),
      port_index_(0) {

  if (target_type() != core::serial) {
    throw std::runtime_error(
        "serial_send_data allows only serial device target");
  }

  buffer buf = skip(income, 2);
  buffer::size_type pos = buf.find_first_of(',');
  buffer valbuf = buf.substr(0, pos);

  if (!util::isInteger(valbuf)) {
    throw make_error(errc::bad_message,
                     "vnigma::das::serial_send_data invalid port index");
  }

  port_index_ = (uint8_t)util::toInteger(valbuf, 10);
}

buffer serial_send_data::as_buffer() const {
  return core::control_message<serial_send_data>::as_buffer(*this);
}

std::string serial_send_data::payload() const {
  return send_data::payload();
}

std::string serial_send_data::port() const {
  return std::to_string((long)port_index_);
}

serial::data serial_send_data::get_data() const {
  return send_data::get_data().get<serial::data>();
}

uint8_t serial_send_data::port_index() const {
  return port_index_;
}

device serial_send_data::get_device() const {
  return core::control_message<serial_send_data>::get_device().value();
}

}}  // namespace vnigma::das
