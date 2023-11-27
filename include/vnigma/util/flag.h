#if !defined(VNIGMA_UTIL_FLAG_H)
#define VNIGMA_UTIL_FLAG_H

#include <type_traits>

namespace vnigma {
using Greedy = std::true_type;
using Lavish = std::false_type;
}  // namespace vnigma

#endif  // VNIGMA_UTIL_FLAG_H
