#if !defined(VNIGMA_MESSAGE_DIGITAL_SEND_DATA_H)
#define VNIGMA_MESSAGE_DIGITAL_SEND_DATA_H

#include <vnigma/message/das_send_data.h>
#include <vector>

namespace vnigma { namespace das {
class digital_send_data : public core::control_message<digital_send_data>,
                          public send_data {
 public:
  digital_send_data(device dev, digital::data data);
  digital_send_data(buffer buf);

  virtual ~digital_send_data() = default;

 public:
  buffer as_buffer() const;

  std::string payload() const override;

  device get_device() const;

  digital::data get_data() const;
};
}}  // namespace vnigma::das

#endif  // VNIGMA_MESSAGE_DIGITAL_SEND_DATA_H
