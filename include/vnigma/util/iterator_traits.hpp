#if !defined(VNIGMA_UTIL_ITERATOR_TRAITS_HPP)
#define VNIGMA_UTIL_ITERATOR_TRAITS_HPP

#include <type_traits>

namespace vnigma {
template <typename...>
using void_t = void;

template <typename T, typename = void>
struct is_input_iterator : std::false_type {};

template <typename T>
struct is_input_iterator<
    T, void_t<decltype(++std::declval<T&>()),  // incrementable
              decltype(*std::declval<T&>()),   // dereferencable
              decltype(std::declval<T&>() == std::declval<T&>())  // comparable
              >> : std::true_type {};

// is_input_iterator value helper
template <typename T>
inline constexpr bool is_input_iterator_v = is_input_iterator<T>::value;
}  // namespace vnigma

#endif  // VNIGMA_UTIL_ITERATOR_TRAITS_HPP
