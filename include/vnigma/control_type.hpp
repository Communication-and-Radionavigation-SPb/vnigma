#if !defined(VNIGMA_CONTROL_TYPE_HPP)
#define VNIGMA_CONTROL_TYPE_HPP

#include <vnigma/export.h>
#include <sstream>

namespace vnigma {

namespace das {
class get_config;
class send_data;
class set_config;
class set_frequency;
class set_reset;
}  // namespace das

/**
 * @brief If you see this that means that passed @link vnigma::core::control type
 * is not covered.
 * 
 * @tparam T Control value to receive string */
template <typename Message>
struct control_str {};

/**
 * @brief Send Data to string conversion
 * @link vnigma::core::control_type
 */
template <>
struct control_str<das::send_data> {
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

}  // namespace vnigma

#endif  // VNIGMA_CONTROL_TYPE_HPP
