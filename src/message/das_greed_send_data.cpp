#include <vnigma/message/das_greed_send_data.h>
#include <vnigma/util/buffer_manip.h>

namespace vnigma { namespace das {

greed_send_data::greed_send_data(device dev, data_variant data)
    : core::control_message<greed_send_data>(dev), send_data(std::move(data)) {}

greed_send_data::greed_send_data(buffer buf)
    : core::control_message<greed_send_data>(buf),
      send_data(std::move(buf), target_type()) {}

buffer greed_send_data::as_buffer() const {
  return core::control_message<greed_send_data>::as_buffer(*this);
}

std::string greed_send_data::payload() const {
  return send_data::payload();
}

device greed_send_data::get_device() const {
  return core::control_message<greed_send_data>::get_device().value();
}

}}  // namespace vnigma::das
