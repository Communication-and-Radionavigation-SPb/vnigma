#if !defined(VNIGMA_CONTROL_MESSAGE)
#define VNIGMA_CONTROL_MESSAGE

/* ----------------------------------- STD ---------------------------------- */
#include <cctype>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
/* -------------------------------- Internal -------------------------------- */
#include <vnigma/device.h>
#include <vnigma/id_t.h>
#include <vnigma/types.h>
#include <vnigma/util/variant.h>
#include <vnigma/buffer.hpp>
#include <vnigma/control_type.hpp>
#include <vnigma/message/message_traits.hpp>

namespace vnigma { namespace core {

template <typename Message>
class VNIGMA_EXPORT control_message {
  /* ------------------------------- Constructor ------------------------------ */
 public:
  control_message(device dev) : device_(dev) {}

  control_message(buffer buf) {
    throw_if_empty(buf, errc::protocol_error, "header is apsent");
    /* ----------------------------- Validate prefix ---------------------------- */
    buffer::iterator lpos = buf.begin();
    buffer::iterator rpos = --buf.end();
    if (*lpos++ != '<') {
      error(errc::bad_message, "wrong prefix");
    }
    if (*rpos-- != '\n') {
      error(errc::bad_message, "no line ending");
    }
    if (*rpos-- != '\r') {
      error(errc::bad_message, "no carret return");
    }
    /* ------------------------ validate protocol version ----------------------- */
    if constexpr (das_related<Message>()) {
      if (lpos == buf.end() || buf.compare(lpos - buf.begin(), 2, "DS") != 0) {
        std::stringstream ss;
        ss << "invalid protocol version" << buf.substr(lpos, 2);
        error(errc::protocol_error, ss.str());
      }
    }
    else {
      if (lpos == buf.end() || buf.compare(lpos - buf.begin(), 2, "VN") != 0) {
        std::stringstream ss;
        ss << "invalid protocol version" << buf.substr(lpos, 2);
        error(errc::protocol_error, ss.str());
      }
    }
    lpos = lpos + 2;  // move pos forward
    // <DSSSD,582,3,2,$GPHDT,127.0,T*ce\r\n
    //    ^                           ^
    //    l                           r
    /* -------------------- Validate and extract device type -------------------- */
    core::Type device_type;
    try {
      device_type = type_from_string(*lpos++);
    } catch (const std::invalid_argument& e) {
      error(errc::bad_message, e.what());
    }
    // <DSSSD,582,3,2,$GPHDT,127.0,T*ce\r\n
    //     ^                          ^
    //     l                          r
    const auto cmd = control_str<Message>::value;
    // validate command equals to the type from ^^^^
    /* ------------------------ validate payload present ------------------------ */
    if constexpr (has_payload<Message>()) {}

    /* ------------------------- validate uuid if needed ------------------------ */
    if constexpr (das_related<Message>()) {
      lpos = std::min(buf.find(','), buf.size());  // find left bound of uid
      rpos = std::min(buf.find(',', lpos + 1),
                      buf.size());  // find right bound of uid
      if (lpos == buffer::npos || rpos == buffer::npos || lpos == rpos) {
        error(errc::bad_message, "uuid apsent");
      }
      lpos = rpos;
    }
    /* --------------------- validate and extract device id --------------------- */
    if (lpos == buffer::npos || buf.size() <= (lpos + 1)) {
      error(errc::bad_message, "device identifier apsent");
    }
    buf.remove_prefix(lpos + 1);  // forget everything before device id

    rpos = buf.find_first_of(",\r");  // find right bound

    auto sub = buf.substr(0, rpos);  // content

    char* p;
    std::string idstr{sub.begin(), sub.size()};
    uint8_t id = std::strtoul(idstr.c_str(), &p, 10);
    if (*p != 0 || idstr.empty()) {
      error(errc::bad_message, "could not extract device identifier");
    }
    device_ = device(id, device_type);

    buf.remove_prefix(rpos);

    /* ----------------- validate has port field (empty or not) ----------------- */
    if constexpr (is_port_missed<Message>() || is_port_scoped<Message>()) {

      // TODO:
    }

    /* ------------------- validate that payload is presented ------------------- */
    /*                      if message marked as payloadfull                      */
    if constexpr (has_payload<Message>()) {
      // TODO:
    }
  }

 private:
  [[noreturn]] void error(errc ec, const std::string& error) {
    constexpr auto type = control_str<Message>::value;
    std::stringstream ss;
    ss << "vnigma::control_message " << type << " " << error;
    throw make_error(ec, ss.str());
  }
  void throw_if_empty(buffer& buf, errc ec, const std::string& ctnt) {
    if (buf.empty()) {
      error(ec, ctnt);
    }
  }

  /* -------------------------------- As buffer ------------------------------- */
 protected:
  /**
   * @brief  convert message to buffer
   * 
   * @tparam Message message type
   * @param message Message to be converted.
   * @throws std::runtime_error If passed message can not be converted
   *                            to buffer
   * @return buffer command buffer representation
   */
  buffer as_buffer(Message message) {
    if constexpr (!is_message_variant<Message>()) {  // #message_validation
      static_assert(is_command<Message>() || is_response<Message>(),
                    "Message can not be converted to buffer");
    }

    std::stringstream ss;

    if constexpr (is_message_variant<Message>()) {
      message.visit(overload{[&](auto& actual) {
                               // translate actual message to string
                               as_buffer(message, ss);
                             },
                             [](system_error&) {
                               // will never be called
                               // because of #message_validation
                             }});
    }
    else {
      as_buffer(message, ss);
    }

    std::string result = ss.str();
    return allocate_buffer(result);
  }

 private:
  std::ostream& as_buffer(Message& message, std::ostream& ss) {
    constexpr auto type = control_str<Message>::value;
    // Compose with target module protocol and device properties
    if constexpr (das_related<Message>())
      ss << "<DS";

    if constexpr (venom_related<Message>())
      ss << "<VN";

    ss << core::type_to_string(device_.value().type()) << type;

    if constexpr (das_related<Message>()) {
      ss << "," << message.get_uuid();
    }

    ss << "," << (unsigned)device_.value().id();

    // port separator if needed
    if constexpr (is_port_missed<Message>() || is_port_scoped<Message>()) {
      ss << ",";
    }
    // port index if needed
    if constexpr (is_port_scoped<Message>()) {
      ss << message.port();
    }
    // payload if needed
    if constexpr (has_payload<Message>()) {
      ss << "," << message.payload();
    }
    ss << "\r\n";
    return ss;
  }
  /* ------------------------------- Attributes ------------------------------- */
 private:
  std::optional<device> device_;
};
}}  // namespace vnigma::core

#endif  // VNIGMA_CONTROL_MESSAGE
