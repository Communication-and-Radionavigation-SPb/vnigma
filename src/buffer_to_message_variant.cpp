#include <vnigma/buffer_to_message_variant.h>
#include <vnigma/device.h>
#include <vnigma/types.h>
#include <vnigma/exception.hpp>

namespace vnigma {

// core::control get_control(buffer buf) {}



message_variant buffer_to_message_variant(buffer buf) {
  if (buf[0] != '<') {
    return make_error(errc::bad_message, "message must start with `<` sign");
  }
  // <VN_**
  //      ^
  //      6
  if (buf.size() < 7) {
    return make_error(errc::message_size, "message size is too short");
  }

  buffer control_buf = buf.substr(0, 6);
  control_buf.remove_prefix(4);
  try {
    // switch (get_control(force_move(control_buf))) {
    //   case core::control::SF:
    //     return set_reset(force_move(buf));
    //     break;
    //   default:
    //     break;
    // }
    return make_error(errc::not_supported, "control is invalid");
  } catch (system_error const& se) {
    return se;
  }
}
}  // namespace vnigma
