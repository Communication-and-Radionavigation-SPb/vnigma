#if !defined(VNIGMA_MESSAGE_SET_FREQUENCY_H)
#define VNIGMA_MESSAGE_SET_FREQUENCY_H

/* ----------------------------------- STD ---------------------------------- */
#include <sstream>
#include <string>
/* -------------------------------- Internal -------------------------------- */
#include <vnigma/message/das_message.h>
#include <vnigma/types.h>
#include <vnigma/message/control_message.hpp>
/* -------------------------------------------------------------------------- */
/*                            Set frequency command                           */
/* -------------------------------------------------------------------------- */
namespace vnigma { namespace das {
/**
 * @brief configures frequncy with which module
 * will send data
 */
class set_frequency : public core::control_message<set_frequency>, public base {
 public:
  using sendp = core::send_period;

 public:
  sendp period;

 public:
  /**
   * @brief creates set_frequency message with default period
   */
  set_frequency(uuid id, device dev);
  /**
   * @brief creates set_frequency message with passed period
   * @param period period of data sending to set for device
   */
  set_frequency(uuid id, device dev, sendp a_period);

  set_frequency(buffer buf);

 public:
  buffer as_buffer() const;

  std::string payload() const;

  device get_device() const;
};
}}  // namespace vnigma::das

#endif  // VNIGMA_MESSAGE_SET_FREQUENCY_H
