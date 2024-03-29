#pragma once

/* -------------------------------- Internal -------------------------------- */
#include <vnigma/export.h>
#include <vnigma/message/das_message.h>
#include <vnigma/buffer.hpp>
#include <vnigma/control_type.hpp>
#include <vnigma/message/control_message.hpp>

#if !defined(VNIGMA_MESSAGES_SET_RESET_H)
#define VNIGMA_MESSAGES_SET_RESET_H

namespace vnigma { namespace das {

/**
 * @brief command to reset target module
 */
class VNIGMA_EXPORT set_reset : public core::control_message<set_reset>,
                                public base {
 public:
  set_reset(uuid id, device dev);
  set_reset(buffer buf);

 public:
  buffer as_buffer() const;

  device get_device() const;
};
}}  // namespace vnigma::das

#endif  // VNIGMA_MESSAGES_SET_RESET_H
