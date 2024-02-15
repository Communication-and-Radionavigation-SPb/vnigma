#include <vnigma/util/buffer_manip.h>
#include <vnigma/message/das_send_data.h>

namespace vnigma { namespace das {

send_data::send_data(data_variant data) : var_(std::move(data)) {}

send_data::send_data(buffer buf, Type target_type) : var_(std::nullopt) {
  // trim terminating simbols
  buf = trim_buffer(buf);

  if (target_type == Type::serial) {
    var_.emplace(serial::data(skip(buf, 3)));
    return;
  }

  // skip header, module index, missed port
  buf = skip(buf, 3);

  if (target_type == Type::analog) {
    var_.emplace(analog::data(buf));
    return;
  }

  if (target_type == Type::digital) {
    buffer::size_type lpos = buf.find_last_of(',');
    var_.emplace(digital::data(buf.substr(lpos + 1)));
    return;
  }

  throw make_error(errc::bad_message, "vnigma::das::send_data unsupported device type data send");
}

std::string send_data::payload() const {
  return var_.value().visit(overload{[](auto& v) {
    return (std::string)v;
  }});
}

data_variant send_data::get_data() const {
  return var_.value();
}

}}  // namespace vnigma::das
