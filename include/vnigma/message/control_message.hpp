#if !defined(VNIGMA_CONTROL_MESSAGE)
#define VNIGMA_CONTROL_MESSAGE

/* ----------------------------------- STD ---------------------------------- */
#include <cctype>
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
#include <vnigma/util/number.hpp>

namespace vnigma { namespace core {

template <typename Message>
class VNIGMA_EXPORT control_message {
  /* ------------------------------- Constructor ------------------------------ */
 public:
  control_message(device dev) : device_(dev) {}

  control_message(buffer& income) {
    auto buf = income.substr();
    throw_if_empty(buf, errc::protocol_error, "header is apsent");
    /* ----------------------------- Validate prefix ---------------------------- */
    if (buf.at(0) != '<') {
      error(errc::bad_message, "wrong prefix");
    }
    auto lf_pos = buf.rfind('\n');
    auto cr_pos = buf.rfind('\r');

    if (lf_pos != (buf.size() - 1)) {
      error(errc::bad_message, "no line ending");
    }
    if (cr_pos != (buf.size() - 2)) {
      error(errc::bad_message, "no carret return");
    }
    /* ------------------------ retrieve target protocol ------------------------ */
    std::string protocol = "";
    if constexpr (das_related<Message>()) {
      protocol = "DS";
    }
    else if constexpr (venom_related<Message>()) {
      protocol = "VN";
    }
    /* ------------------------ validate protocol version ----------------------- */
    adjust_left(buf, 1, errc::bad_message, "message is too short");
    adjust_right(buf, 2, errc::bad_message, "message is too short");
    if (buf.compare(0, 2, protocol) != 0) {
      std::stringstream ss;
      ss << "invalid protocol version '" << buf.substr(0, 2) << "'";
      error(errc::protocol_error, ss.str().c_str());
    }
    /* -------------------- Validate and extract device type -------------------- */
    buf.remove_prefix(2);
    throw_if_empty(buf, errc::protocol_error, "no device clarifier");
    core::Type device_type;
    try {
      device_type = type_from_char(buf.at(0));
    } catch (const std::invalid_argument& e) {
      error(errc::bad_message, e.what());
    }
    if (is_service<Message>() && device_type != Type::general) {
      error(errc::bad_message,
            "service message must be targeted to general type which 'X'");
    }
    /* ------------------------- validate message format ------------------------ */
    buf.remove_prefix(1);
    throw_if_empty(buf, errc::bad_message, "format token apsent");
    if (buf.compare(0, 2, control_str<Message>::value) != 0) {
      error(errc::bad_message, "message format token not matches target one");
    }
    /* ------------------------- adjust to next section ------------------------- */
    buf.remove_prefix(2);
    /* ----------------------------- searching ptrs ----------------------------- */
    buffer::size_type lpos = 0;
    buffer::size_type rpos = 0;
    /* ---------------------- verify uuid present if needed --------------------- */
    if constexpr (das_related<Message>() && !is_response<Message>() &&
                  !is_service<Message>()) {
      // validate delimeter
      if (buf.at(lpos) != ',') {
        error(errc::bad_message, "message is malformed");
      }
      // find right bound of uid
      rpos = std::min(buf.find(',', lpos + 1), buf.size());
      auto sub = buf.substr(lpos + 1, rpos - lpos - 1);
      // verify uuid present
      if (sub.empty()) {
        error(errc::bad_message, "uuid apsent");
      }
      // verify contents
      if (!util::isInteger(sub)) {
        error(errc::bad_message, "uuid invalid");
      }
      lpos = rpos;
    }
    /* --------------------- validate and extract device id --------------------- */
    device::id_t devid = 0;
    if (!is_service<Message>()) {
      if (lpos == buf.size()) {
        error(errc::bad_message, "device identifier apsent");
      }
      // find right bound
      rpos = std::min(buf.find_first_of(",", lpos + 1), buf.size());
      auto sub = buf.substr(lpos + 1, rpos - lpos - 1);  // content
      if (sub.empty()) {
        error(errc::bad_message, "device identifier empty");
      }
      char* p;
      std::string idstr{sub.begin(), sub.size()};

      devid = std::strtoul(idstr.c_str(), &p, 10);
      if (*p != 0) {
        error(errc::bad_message, "device identifier invalid");
      }
      lpos = rpos;
    }
    device_ = device(devid, device_type);

    /* ----------------- validate has port field (empty or not) ----------------- */
    if constexpr (is_port_missed<Message>() || is_port_scoped<Message>()) {
      if (lpos == buf.size()) {
        error(errc::bad_message, "port field missed");
      }
      rpos = std::min(buf.find_first_of(',', lpos + 1), buf.size());
    }
    if constexpr (is_port_missed<Message>()) {
      if (!buf.substr(lpos + 1, rpos - lpos - 1).empty()) {
        error(errc::bad_message, "invalid port field");
      }
    }
    if constexpr (is_port_scoped<Message>()) {
      auto sub = buf.substr(lpos + 1, rpos - lpos - 1);
      if (sub.empty()) {
        error(errc::bad_message, "port field empty");
      }
      if (!util::isInteger(sub)) {
        error(errc::bad_message, "invalid port field");
      }
    }
    lpos = rpos;

    /* ------------------- validate that payload is presented ------------------- */
    /*                      if message marked as payloadfull                      */
    if constexpr (has_payload<Message>()) {
      if (lpos == buf.size()) {
        error(errc::bad_message, "payload apsent");
      }
      if (buf.substr(lpos + 1, buf.size() - lpos).empty()) {
        error(errc::bad_message, "payload empty");
      }
    }
  }

 protected:
  std::optional<device> get_device() const { return device_; }

 private:
  /**
   * @brief throws system_error with code and content
   * passed as params
   * @throws always
   * @param ec error code
   * @param error description of failure
   */
  void error(errc ec, const char* error) {
    constexpr auto type = control_str<Message>::value;
    std::stringstream ss;
    ss << "vnigma::control_message " << type << " " << error;
    throw make_error(ec, ss.str());
  }
  void throw_if_empty(buffer& buf, errc ec, const char* ctnt) {
    if (buf.empty()) {
      error(ec, ctnt);
    }
  }
  void adjust_left(buffer& buf, buffer::size_type size, errc ec,
                   const char* cntnt) {
    if (buf.size() < size) {
      error(ec, cntnt);
    }
    buf.remove_prefix(size);
  }
  void adjust_right(buffer& buf, buffer::size_type size, errc ec,
                    const char* cntnt) {
    if (buf.size() < size) {
      error(ec, cntnt);
    }
    buf.remove_suffix(size);
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
  buffer as_buffer(Message message) const {
    if constexpr (!is_message_variant<Message>()) {  // #message_validation
      static_assert(is_command<Message>() || is_response<Message>() ||
                        is_service<Message>(),
                    "Message can not be converted to buffer");
    }

    std::stringstream ss;

    if constexpr (is_message_variant<Message>()) {
      message.visit(overload{[&](auto& actual)
                             {
                               // translate actual message to string
                               as_buffer(message, ss);
                             },
                             [](system_error&)
                             {
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

  Type target_type() const { return device_.value().type(); }

 private:
  std::ostream& as_buffer(Message& message, std::ostream& ss) const {
    constexpr auto type = control_str<Message>::value;
    // Compose with target module protocol and device properties
    if constexpr (das_related<Message>())
      ss << "<DS";

    if constexpr (venom_related<Message>())
      ss << "<VN";

    ss << core::type_to_string(device_.value().type()) << type;

    if constexpr (das_related<Message>() && is_command<Message>()) {
      ss << "," << message.get_uuid();
    }
    if constexpr (!is_service<Message>()) {
      ss << "," << std::to_string(device_.value().id());
    }

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
