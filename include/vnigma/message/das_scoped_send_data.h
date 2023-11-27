#if !defined(VNIGMA_MESSAGE_SCOPED_SEND_DATA)
#define VNIGMA_MESSAGE_SCOPED_SEND_DATA

#include <vnigma/message/das_send_data.h>

namespace vnigma { namespace das {

class scoped_send_data : public core::control_message<scoped_send_data>,
                         public send_data {
 public:

  scoped_send_data(uint8_t index, device dev, data_variant data);
  scoped_send_data(buffer buf);

 public:
  buffer as_buffer();

  std::string payload() override;

  std::string port();
 private:
  uint8_t port_index_;
};

}}  // namespace vnigma::das

#endif  // VNIGMA_MESSAGE_SCOPED_SEND_DATA
