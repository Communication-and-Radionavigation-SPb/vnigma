#include <vnigma/message/das_send_data.h>

namespace vnigma { namespace das {
send_data::send_data(uuid id, device dev)
    : core::control_message<send_data>(dev), base(id) {}
}}  // namespace vnigma::das
