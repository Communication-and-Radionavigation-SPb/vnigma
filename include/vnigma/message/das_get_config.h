#pragma once

/* -------------------------------- Internal -------------------------------- */
#include <vnigma/export.h>
#include <vnigma/message/das_message.h>
#include <vnigma/message/control_message.hpp>

#if !defined(VNIGMA_MESSAGE_GET_CONFIG_H)
#define VNIGMA_MESSAGE_GET_CONFIG_H

namespace vnigma { namespace das {

class VNIGMA_EXPORT get_config : public core::control_message<get_config>,
                                 public base {
 public:
  /**
   * @brief get config command
   * 
   * @param id command identifier
   * @param dev target device
   */
  get_config(uuid id, device dev);

  /**
   * @brief get config command from buffer
   * @param buf buffer to parse
   */
  get_config(buffer buf);

 public:
  /**
   * @brief get_config command to buffer conversion
   * @return buffer buffer with get_config command contents
   */
  buffer as_buffer() const;

  device get_device() const;
};
}}  // namespace vnigma::das

#endif  // VNIGMA_MESSAGE_GET_CONFIG_H
