#if !defined(VNIGMA_BAUD_RATE_H)
#define VNIGMA_BAUD_RATE_H

#include <vnigma/export.h>
#include <cstdint>

namespace vnigma { namespace serial {
/**
 * @brief supported baud rates
 * 
 */
enum VNIGMA_EXPORT BaudRate : uint32_t {
  /** @brief Baud rate with 2400 speed */
  Baud2400 = 0x1,
  /** @brief Baud rate with 4800 speed */
  Baud4800 = 0x2,
  /** @brief Baud rate with 9600 speed */
  Baud9600 = 0x3,
  /** @brief Baud rate with 19200 speed */
  Baud19200 = 0x4,
  /** @brief Baud rate with 38400 speed */
  Baud38400 = 0x5,
  /** @brief Baud rate with 115200 speed */
  Baud115200 = 0x6,
};

template <BaudRate T>
struct bin_baud_rate {};

template <>
struct bin_baud_rate<Baud2400> {
  static constexpr unsigned value = 0x1;
};

template <>
struct bin_baud_rate<Baud4800> {
  static constexpr unsigned value = 0x2;
};

template <>
struct bin_baud_rate<Baud9600> {
  static constexpr unsigned value = 0x3;
};

template <>
struct bin_baud_rate<Baud19200> {
  static constexpr unsigned value = 0x4;
};

template <>
struct bin_baud_rate<Baud38400> {
  static constexpr unsigned value = 0x5;
};

template <>
struct bin_baud_rate<Baud115200> {
  static constexpr unsigned value = 0x6;
};

}}  // namespace vnigma::serial

#endif  // VNIGMA_BAUD_RATE_H
