#if !defined(VNIGMA_UTIL_MOVE_HPP)
#define VNIGMA_UTIL_MOVE_HPP

#include <type_traits>
#include <utility>

namespace vnigma {
template <typename T>
constexpr typename std::remove_reference_t<T>&& force_move(T&& t) {
  static_assert(!std::is_const_v<std::remove_reference_t<T>>,
                "T is const. Fallback to copy.");
  return std::move(t);
}
}  // namespace vnigma

#endif  // VNIGMA_UTIL_MOVE_HPP
