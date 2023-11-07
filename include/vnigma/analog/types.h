#pragma once

#include <vnigma/export.h>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace vnigma { namespace analog {

/**
 * @brief Analog port measure mode state configuration value
 */
VNIGMA_EXPORT enum Runmode : uint8_t {
  mA0_20R250, /** Current loop mode with 0 to 20 mA measure range and 250 Om resistance*/
  mA0_20R500, /** Current loop mode with 0 to 20 mA measure range and 500 Om resistance*/
  mA0_20R1K, /** Current loop mode with 0 to 20 mA measure range and 1K Om resistance*/
  V10_10R47K, /** Voltage measure from -10V up to 10V with 47.000 Om resitance*/
};

namespace data {
VNIGMA_EXPORT enum Period {
  never = -1,
  fastest = 500,
  fast = 1000,
  normal = 2000,
  slow = 5000,
  slower = 10000,
};

Period from_string(const std::string& str);
std::string to_string(const Period& d);
}  // namespace data
namespace config {
VNIGMA_EXPORT enum Period {
  never = -1,
  fastest = 2,
  fast = 5,
  slow = 10,
  slower = 60
};

Period from_string(const std::string& str);
std::string to_string(const Period& d);
}  // namespace config

#define ANALOG_PORTS_COUNT 8

}}  // namespace vnigma::analog
