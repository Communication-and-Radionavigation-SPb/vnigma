#include <vnigma/message/das_scoped_send_data.h>
#include <vnigma/util/buffer_manip.h>
#include <vnigma/util/number.hpp>

namespace vnigma { namespace das {

scoped_send_data::scoped_send_data(uint8_t index, device dev, data_variant data)
    : core::control_message<scoped_send_data>(dev),
      send_data(std::move(data)),
      port_index_(index) {}

scoped_send_data::scoped_send_data(buffer income)
    : core::control_message<scoped_send_data>(income),
      send_data(income, target_type()),
      port_index_(0) {

  buffer buf = skip(income, 2);
  buffer::size_type pos = buf.find_first_of(',');
  buffer valbuf = buf.substr(0, pos);

  if (!util::isInteger(valbuf)) {
    throw make_error(errc::bad_message,
                     "vnigma::das::scoped_send_data invalid port index");
  }

  port_index_ = (uint8_t)util::toInteger(valbuf, 10);
}

buffer scoped_send_data::as_buffer() const {
  return core::control_message<scoped_send_data>::as_buffer(*this);
}

std::string scoped_send_data::payload() const {
  return send_data::payload();
}

std::string scoped_send_data::port() const {
  return std::to_string((long)port_index_);
}

}}  // namespace vnigma::das
