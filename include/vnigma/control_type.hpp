#if !defined(VNIGMA_CONTROL_TYPE_HPP)
#define VNIGMA_CONTROL_TYPE_HPP

#include <vnigma/export.h>
#include <sstream>

namespace vnigma {

namespace das {
class get_config;
class analog_send_data;
class serial_send_data;
class digital_send_data;
class set_config;
class set_frequency;
class set_reset;
class ack;
class handshake;
}  // namespace das

/**
 * @brief If you see this that means that passed @link vnigma::core::control type
 * is not covered.
 * 
 * @tparam T Control value to receive string */
template <typename Message>
struct control_str {};

/**
 * @brief Analog send data message format string
 * @link vnigma::core::control_type
 */
template <>
struct control_str<das::analog_send_data> {
  static constexpr char value[] = "SD";
};

/**
 * @brief Serial send data message format string
 * @link vnigma::core::control_type
 */
template <>
struct control_str<das::serial_send_data> {
  static constexpr char value[] = "SD";
};

/**
 * @brief Digital send data message format string
 * @link vnigma::core::control_type
 */
template <>
struct control_str<das::digital_send_data> {
  static constexpr char value[] = "SD";
};

/**
 * @brief Set Frequency to string conversion
 * @link vnigma::core::control_type
 */
template <>
struct control_str<das::set_frequency> {
  static constexpr char value[] = "SF";
};

/**
 * @brief Set Reset to string conversion
 * @link vnigma::core::control_type
 */
template <>
struct control_str<das::set_reset> {
  static constexpr char value[] = "SR";
};

/**
 * @brief Set config to string conversion
 * @link vnigma::core::control_type
 */
template <>
struct control_str<das::set_config> {
  static constexpr char value[] = "SC";
};

template <>
struct control_str<das::get_config> {
  static constexpr char value[] = "GC";
};

template <>
struct control_str<das::ack> {
  static constexpr char value[] = "AK";
};

template <>
struct control_str<das::handshake> {
  static constexpr char value[] = "SN";
};

}  // namespace vnigma

#endif  // VNIGMA_CONTROL_TYPE_HPP
