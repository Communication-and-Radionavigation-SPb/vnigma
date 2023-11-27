#if !defined(VNIGMA_UTIL_VARIANT_H)
#define VNIGMA_UTIL_VARIANT_H

#include <variant>

namespace vnigma {
using std::monostate;
using std::variant;
using std::visit;

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

}  // namespace vnigma

#endif  // VNIGMA_UTIL_VARIANT_H
