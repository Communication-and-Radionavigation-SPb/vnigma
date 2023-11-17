#if !defined(VNIGMA_PORTS_SERIAL_PORT_H)
#define VNIGMA_PORTS_SERIAL_PORT_H
/* ----------------------------------- STD ---------------------------------- */
#include <cstdint>
/* -------------------------------- Internal -------------------------------- */
#include <vnigma/baud.h>

/* -------------------------------------------------------------------------- */
/*                             Vnigma serial port                             */
/* -------------------------------------------------------------------------- */
namespace vnigma { namespace serial {
class port {
 public:
  uint8_t index;
  uint8_t configuration;

 public:
  /**
   * @brief create port object with default configuration.
   * port is `enabled` and baud rate is `2400` 
   * 
   * @param index port index
   */
  port(uint8_t index);
  port(uint8_t index, uint8_t config);

 public:
  bool is_enabled() const noexcept;
  BaudRate baud_rate() const noexcept;

  void set_baud_rate(BaudRate) noexcept;
  void enable() noexcept;
  void disable() noexcept;
};
}}  // namespace vnigma::serial

#endif  // VNIGMA_PORTS_SERIAL_PORT_H
