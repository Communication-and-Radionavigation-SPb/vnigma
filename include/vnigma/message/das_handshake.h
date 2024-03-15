#pragma once

#include <vnigma/export.h>
#include <vnigma/message/das_message.h>
#include <string>
#include <vnigma/buffer.hpp>
#include <vnigma/message/control_message.hpp>
#include <vnigma/util/move.hpp>

#if !defined(VNIGMA_MESSAGE_DAS_HANDSHAKE)
#define VNIGMA_MESSAGE_DAS_HANDSHAKE

namespace vnigma { namespace das {
class VNIGMA_EXPORT handshake : public core::control_message<handshake> {
 public:
  using serial_number_t = std::string;

 public:
  handshake(serial_number_t);
  handshake(buffer buf);

 public:
  serial_number_t serial_number() const;

 public:
  buffer as_buffer() const;

  std::string payload() const;

 private:
  /** @brief Serial number */
  serial_number_t sn_;
};
}}  // namespace vnigma::das

#endif  // VNIGMA_MESSAGE_DAS_HANDSHAKE
