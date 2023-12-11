#pragma once

#include <vnigma/export.h>
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>

namespace vnigma { namespace core {

/**
 * @brief Supported data transmission periods
 */
enum VNIGMA_EXPORT send_period : int16_t {
  /** @brief 500 ms send period. */
  fast = 500,
  /** @brief 1 second send period. Default. */
  normal = 1000,
  /** @brief 2 seconds send period. */
  slow = 2000,
  /** @brief 5 seconds send period. */
  slower = 5000,
  /** @brief 10  seconds send period.*/
  slowest = 10000,
  /** @brief device will never send any data untill new period is set */
  never = -1,
};
/**
 * @brief Module type
 * 
 */
enum VNIGMA_EXPORT Type { analog, serial, digital, general };

/**
 * @brief Converts module <Type> to string which
 * contains module type letter
 * 
 * @param t Type of module
 * @return std::string String representation of module type
 */
std::string type_to_string(const Type& t) noexcept;

/**
 * @brief Converts string representation of module
 * type into <vnigma::core::Type> type
 * 
 * @param str String which contains module type
 * @throws std::invalid_argument if passed parameter do not
 * contains any known module type
 * @return Type Type of module
 */
Type type_from_char(const char);

}}  // namespace vnigma::core
