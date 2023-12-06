#if !defined(VNIGMA_MESSAGE_GREED_SEND_DATA)
#define VNIGMA_MESSAGE_GREED_SEND_DATA

#include <vnigma/message/das_send_data.h>

namespace vnigma { namespace das {

/**
 * @brief port missed send data response
 * used for analog and digital data
 */
class greed_send_data : public core::control_message<greed_send_data>,
                        public send_data {
 public:
  greed_send_data(device dev, data_variant data);
  greed_send_data(buffer buf);

  virtual ~greed_send_data() = default;

 public:
  buffer as_buffer() const;

  std::string payload() const override;
};
}}  // namespace vnigma::das

#endif  // VNIGMA_MESSAGE_GREED_SEND_DATA
