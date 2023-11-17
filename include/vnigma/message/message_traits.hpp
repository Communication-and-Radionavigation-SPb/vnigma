#if !defined(VNIGMA_MESSAGE_TRAITS_HPP)
#define VNIGMA_MESSAGE_TRAITS_HPP

#include <type_traits>
#include <vnigma/exception.hpp>

namespace vnigma {

class message_variant;
namespace das {

class set_reset;
class set_frequency;
class set_config;
class get_config;
}  // namespace das

template <typename Message>
constexpr bool das_related() {
  return std::is_same_v<das::set_reset, Message> ||
         std::is_same_v<das::set_frequency, Message> ||
         std::is_same_v<das::get_config, Message> ||
         std::is_same_v<das::set_config, Message>;
}

template <typename Message>
constexpr bool venom_related() {
  return false;
}

template <typename Message>
constexpr bool is_command() {
  return std::is_same_v<das::set_reset, Message> ||
         std::is_same_v<das::set_frequency, Message> ||
         std::is_same_v<das::set_config, Message> ||
         std::is_same_v<das::get_config, Message>;
}

template <typename Message>
constexpr bool is_port_scoped() {
  return std::is_same_v<das::set_config, Message>;
}

template <typename Message>
constexpr bool is_port_missed() {
  return std::is_same_v<das::set_frequency, Message>;
}

template <typename Message>
constexpr bool has_payload() {
  return std::is_same_v<das::set_frequency, Message> ||
         std::is_same_v<das::set_config, Message>;
}

template <typename Message>
constexpr bool is_message_variant() {
  return std::is_same_v<message_variant, Message>;
}

template <typename Message>
constexpr bool is_response() {
  return false;
}
}  // namespace vnigma

#endif  // VNIGMA_MESSAGE_TRAITS_HPP
