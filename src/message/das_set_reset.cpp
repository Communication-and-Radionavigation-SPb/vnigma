#include <vnigma/message/das_set_reset.h>
#include <vnigma/exception.hpp>

namespace vnigma { namespace das {

set_reset::set_reset(uuid id, device dev)
    : core::control_message<set_reset>(dev), base(id) {}
}}  // namespace vnigma::das
