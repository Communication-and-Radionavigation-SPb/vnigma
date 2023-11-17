#if !defined(VNIGMA_MESSAGE_ANALOG_SEND_DATA_H)
#define VNIGMA_MESSAGE_ANALOG_SEND_DATA_H

/* ----------------------------------- STD ---------------------------------- */
/* -------------------------------- Internal -------------------------------- */
#include <vnigma/message/control_message.hpp>
#include <vnigma/message/das_message.h>

/* -------------------------------------------------------------------------- */
/*                              Analog send data                              */
/* -------------------------------------------------------------------------- */

namespace vnigma { namespace das {

class send_data : public core::control_message<send_data>, public base {
 public:
  send_data(uuid id, device dev);
};
}}  // namespace vnigma::das

#endif  // VNIGMA_MESSAGE_ANALOG_SEND_DATA_H
