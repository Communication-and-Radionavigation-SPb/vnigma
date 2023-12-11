#if !defined(VNIGMA_MESSAGE_SET_CONFIG_H)
#define VNIGMA_MESSAGE_SET_CONFIG_H

/* ----------------------------------- STD ---------------------------------- */
/* -------------------------------- Internal -------------------------------- */
#include <vnigma/message/das_message.h>
#include <vnigma/ports/serial_port.h>
#include <vnigma/buffer.hpp>
#include <vnigma/message/control_message.hpp>
#include <vnigma/util/move.hpp>
/* -------------------------------------------------------------------------- */
/*                             Set config command                             */
/* -------------------------------------------------------------------------- */

/* -------------------------- Serial module target -------------------------- */
// <DSSSC,1,2,5,FC
//        ^ ^ ^ ^
//        1 2 3 4
// 1. Message unique id
// 2. Module id
// 3. Port number
// 4. Configuration
namespace vnigma { namespace das {
class set_config : public core::control_message<set_config>, public base {
 public:
  using port_t = serial::port;

 private:
  port_t p_;

 public:
  set_config(uuid id, device dev, port_t p);
  set_config(uuid id, device dev, uint8_t index);
  set_config(uuid id, device dev, uint8_t index, uint8_t configuration);

  set_config(buffer buf);

 public:
  void set_port(port_t p);
  std::string port() const;
  std::string payload() const;

  buffer as_buffer() const;

  device get_device() const;
};
}}  // namespace vnigma::das

#endif  // VNIGMA_MESSAGE_SET_CONFIG_H
