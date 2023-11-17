#if !defined(VNIGMA_UTIL_BINARY_HPP)
#define VNIGMA_UTIL_BINARY_HPP

namespace vnigma {

template <unsigned long N>
struct binary {
  static unsigned const value = binary<N / 10>::value << 1 | N % 10;
};

template <>
struct binary<0> {
  static unsigned const value = 0;
};


}  // namespace vnigma

#endif  // VNIGMA_UTIL_BINARY_HPP
