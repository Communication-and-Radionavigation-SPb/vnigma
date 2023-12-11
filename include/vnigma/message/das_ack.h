#if !defined(VNIGMA_MESSAGE_ACK_SCOPED_H)
#define VNIGMA_MESSAGE_ACK_SCOPED_H

/* -------------------------------- Internal -------------------------------- */
#include <vnigma/message/das_message.h>
#include <vnigma/message/control_message.hpp>

namespace vnigma { namespace das {
class ack : public core::control_message<ack>, public base {
 public:
  ack(uuid id, device dev, buffer payload);
  ack(buffer buf);

 public:
  buffer as_buffer() const;

  std::string payload() const;

  device get_device() const;

 private:
  buffer payload_;
};
}}  // namespace vnigma::das

#endif  // VNIGMA_MESSAGE_ACK_SCOPED_H
