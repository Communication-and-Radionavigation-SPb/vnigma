#pragma once

#include <vnigma/export.h>
#include <cstdint>
#include <string>

namespace vnigma { namespace digital { namespace data {
VNIGMA_EXPORT enum Period: int16_t {
  never = -1,
  fastest = 500,
  fast = 1000,
  normal = 2000,
  slow = 5000,
  slower = 10000,
};


Period from_string(const std::string& str);
std::string to_string(const Period& period);

}}}  // namespace vnigma::digital::data
