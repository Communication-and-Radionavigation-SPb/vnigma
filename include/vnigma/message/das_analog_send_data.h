#if !defined(VNIGMA_MESSAGE_ANALOG_SEND_DATA)
#define VNIGMA_MESSAGE_ANALOG_SEND_DATA

#include <vnigma/export.h>
#include <vnigma/message/das_send_data.h>
#include <vector>

namespace vnigma { namespace das {

/**
 * @brief port missed send data response
 * used for analog and digital data
 */
class VNIGMA_EXPORT analog_send_data
    : public core::control_message<analog_send_data>,
      public send_data {
 public:
  analog_send_data(device dev, data_variant data);
  analog_send_data(buffer buf);

  virtual ~analog_send_data() = default;

 public:
  buffer as_buffer() const;

  std::string payload() const override;

  device get_device() const;

  analog::data get_data() const;
};
}}  // namespace vnigma::das

#endif  // VNIGMA_MESSAGE_ANALOG_SEND_DATA
