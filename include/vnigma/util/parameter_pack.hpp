#if !defined(VNIGMA_UTIL_PARAMETER_PACK_HPP)
#define VNIGMA_UTIL_PARAMETER_PACK_HPP

namespace vnigma {

template <typename... Ts>
struct length;

template <>
struct length<> {
  static const int value = 0;
};

template <typename T, typename... Ts>
struct length<T, Ts...> {
  static const int value = 1 + length<Ts...>::value;
};

}  // namespace vnigma

#endif  // VNIGMA_UTIL_PARAMETER_PACK_HPP
