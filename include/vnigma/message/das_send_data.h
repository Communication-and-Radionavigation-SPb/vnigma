#if !defined(VNIGMA_MESSAGE_ANALOG_SEND_DATA_H)
#define VNIGMA_MESSAGE_ANALOG_SEND_DATA_H

/* ----------------------------------- STD ---------------------------------- */
#include <vector>
/* -------------------------------- Internal -------------------------------- */
#include <vnigma/message/das_message.h>
#include <vnigma/buffer.hpp>
#include <vnigma/message/control_message.hpp>

/* -------------------------------------------------------------------------- */
/*                             Send data response                             */
/* -------------------------------------------------------------------------- */

namespace vnigma {
/* --------------------------- Send data response --------------------------- */
namespace das {

class send_data : public core::control_message<send_data>, public base {
 public:
  send_data(uuid id, device dev);
  send_data(buffer buf);

 public:
  std::string payload();

  buffer as_buffer();
};
}  // namespace das
}  // namespace vnigma

#endif  // VNIGMA_MESSAGE_ANALOG_SEND_DATA_H
