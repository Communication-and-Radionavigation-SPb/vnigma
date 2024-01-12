#include <vnigma/buffer_to_message_variant.h>
#include <vnigma/device.h>
#include <vnigma/types.h>
#include <vnigma/util/buffer_manip.h>
#include <vnigma/exception.hpp>

namespace vnigma {

/**
 * @brief Returns control message type
 * @return std::string_view 
 */
std::string_view get_control(buffer& buf) {
  return buf.substr(4, 6);
}

/**
 * @brief Returns source device talker plus device type
 * @throws exception if buffer do not contains enought items
 * @return std::string_view 
 */
std::string_view get_prototype(buffer& buf) {
  return buf.substr(1, 3);
}

/**
 * @brief Returns source device type represented by
 * one symbol A | S | D
 * @throws exception if buffer do not contains enought items
 * @return std::string_view 
 */
std::string_view get_target(buffer& buf) {
  return buf.substr(3, 1);
}

message_variant buffer_to_message_variant(buffer buf) {
  if (buf.size() < 7) {
    return make_error(errc::message_size, "message size is too short");
  }

  std::string_view control = buf.substr(4, 2);
  std::string_view protocol = buf.substr(1, 2);

  try {

    if (control == "SD") {
      if (protocol == "DS") {
        std::string_view target = get_target(buf);
        if (target == "A" || target == "D") {
          return das::greed_send_data(force_move(buf));
        }
        if (target == "S") {
          return das::scoped_send_data(force_move(buf));
        }
        std::stringstream ss;
        ss << target << " device type is not supported";
        return make_error(errc::not_supported, ss.str());
      }
    }

    if (control == "AK") {
      if (protocol == "DS") {
        return das::ack(force_move(buf));
      }
    }

    if (control == "SN") {
      if (protocol == "DS") {
        return das::handshake(force_move(buf));
      }
    }

    if (control == "SF") {
      if (protocol == "DS") {
        return das::set_frequency(force_move(buf));
      }
    }

    if (control == "SC") {
      if (protocol == "DS") {
        return das::set_config(force_move(buf));
      }
    }

    if (control == "SR") {
      if (protocol == "DS") {
        return das::set_reset(force_move(buf));
      }
    }

    if (control == "GC") {
      if (protocol == "DS") {
        return das::get_config(force_move(buf));
      }
    }

    return make_error(errc::not_supported, "control is invalid");
  } catch (system_error const& se) {
    return se;
  }
}
}  // namespace vnigma
