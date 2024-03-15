
#pragma once

#include <vnigma/util/flag.h>
#include <type_traits>
#include <vnigma/exception.hpp>

#if !defined(VNIGMA_MESSAGE_TRAITS_HPP)
#define VNIGMA_MESSAGE_TRAITS_HPP

namespace vnigma {

class message_variant;
namespace das {
class send_data;
class set_reset;
class set_frequency;
class set_config;
class get_config;
class serial_send_data;
class analog_send_data;
class digital_send_data;
class ack;
class handshake;
}  // namespace das

template <typename Message>
constexpr bool das_related() {
  return std::is_same_v<das::set_reset, Message> ||
         std::is_same_v<das::set_frequency, Message> ||
         std::is_same_v<das::get_config, Message> ||
         std::is_same_v<das::set_config, Message> ||
         std::is_same_v<das::analog_send_data, Message> ||
         std::is_same_v<das::serial_send_data, Message> ||
         std::is_same_v<das::digital_send_data, Message> ||
         std::is_same_v<das::ack, Message> ||
         std::is_same_v<das::handshake, Message>;
}

template <typename Message>
constexpr bool is_data() {
  return std::is_convertible_v<Message, das::send_data> ||
         std::is_same_v<das::send_data, Message>;
}

template <typename Message>
constexpr bool venom_related() {
  return false;
}

template <typename Message>
constexpr bool is_service() {
  return std::is_same_v<das::handshake, Message>;
}

template <typename Message>
constexpr bool is_response() {
  return std::is_same_v<das::serial_send_data, Message> ||
         std::is_same_v<das::analog_send_data, Message> ||
         std::is_same_v<das::digital_send_data, Message>;
}

template <typename Message>
constexpr bool is_command() {
  return std::is_same_v<das::set_reset, Message> ||
         std::is_same_v<das::set_frequency, Message> ||
         std::is_same_v<das::set_config, Message> ||
         std::is_same_v<das::ack, Message> ||
         std::is_same_v<das::get_config, Message>;
}

template <typename Message>
constexpr bool is_port_scoped() {
  return std::is_same_v<das::set_config, Message> ||
         std::is_same_v<das::serial_send_data, Message>;
}

template <typename Message>
constexpr bool is_port_missed() {
  return std::is_same_v<das::set_frequency, Message> ||
         std::is_same_v<das::analog_send_data, Message> ||
         std::is_same_v<das::ack, Message> ||
         std::is_same_v<das::digital_send_data, Message>;
}

template <typename Message>
constexpr bool has_payload() {
  return std::is_same_v<das::set_frequency, Message> ||
         std::is_same_v<das::set_config, Message> ||
         std::is_same_v<das::serial_send_data, Message> ||
         std::is_same_v<das::analog_send_data, Message> ||
         std::is_same_v<das::ack, Message> ||
         std::is_same_v<das::handshake, Message> ||
         std::is_same_v<das::digital_send_data, Message>;
}

template <typename Message>
constexpr bool is_message_variant() {
  return std::is_same_v<message_variant, Message>;
}

}  // namespace vnigma

#endif  // VNIGMA_MESSAGE_TRAITS_HPP
