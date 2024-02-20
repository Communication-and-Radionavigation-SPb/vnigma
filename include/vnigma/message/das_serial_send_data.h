#if !defined(VNIGMA_MESSAGE_SCOPED_SEND_DATA)
#define VNIGMA_MESSAGE_SCOPED_SEND_DATA

#include <vnigma/message/das_send_data.h>

namespace vnigma { namespace das {

class serial_send_data : public core::control_message<serial_send_data>,
                         public send_data {
 public:
  serial_send_data(uint8_t index, device dev, data_variant data);
  serial_send_data(buffer buf);

  virtual ~serial_send_data() = default;

 public:
  buffer as_buffer() const;

  std::string payload() const override;

  std::string port() const;

  uint8_t port_index() const;

  device get_device() const;

  serial::data get_data() const;

 private:
  uint8_t port_index_;
};

}}  // namespace vnigma::das

#endif  // VNIGMA_MESSAGE_SCOPED_SEND_DATA
