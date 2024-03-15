#pragma once
/* ----------------------------------- STD ---------------------------------- */
#include <optional>
/* -------------------------------- Internal -------------------------------- */
#include <vnigma/export.h>
#include <vnigma/message/das_ack.h>
#include <vnigma/message/das_analog_send_data.h>
#include <vnigma/message/das_digital_send_data.h>
#include <vnigma/message/das_get_config.h>
#include <vnigma/message/das_handshake.h>
#include <vnigma/message/das_serial_send_data.h>
#include <vnigma/message/das_set_config.h>
#include <vnigma/message/das_set_frequency.h>
#include <vnigma/message/das_set_reset.h>
#include <vnigma/util/variant.h>
#include <vnigma/control_type.hpp>
#include <vnigma/exception.hpp>

#if !defined(VNIGMA_MESSAGE_VARIANT_H)
#define VNIGMA_MESSAGE_VARIANT_H

namespace vnigma {

using std::optional;

/**
 * @brief Base class for all interaction messages
 * @details All variants except one that represents system error
 *  contain next operations:
 *  - payload()
 *  - type getter 
 * 
 */
class VNIGMA_EXPORT message_variant {
 public:
  template <
      typename Message,
      std::enable_if_t<!std::is_same_v<std::decay_t<Message>, message_variant>,
                       std::nullptr_t>* = nullptr>
  message_variant(Message&& msg) : var_(std::forward<Message>(msg)) {}

  template <typename Func>
  auto visit(Func&& func) const& {
    return vnigma::visit(std::forward<Func>(func), var_);
  }

  template <typename Func>
  auto visit(Func&& func) & {
    return vnigma::visit(std::forward<Func>(func), var_);
  }

  template <typename Func>
  auto visit(Func&& func) && {
    return vnigma::visit(std::forward<Func>(func), var_);
  }

  /**
   * @brief get actual message by type
   * @return actual message
   */
  template <typename T>
  decltype(auto) get() {
    return std::get<T>(var_);
  }

  /**
   * @brief get actual message by type
   * @return actual message
   */
  template <typename T>
  decltype(auto) get() const {
    return std::get<T>(var_);
  }

  /**
   * @brief get actual message pointer by type
   * @return actual message pointer 
   */
  template <typename T>
  decltype(auto) get_if() {
    return std::get_if<T>(&var_);
  }

  /**
   * @brief get actual message pointer by type
   * @return actual message pointer 
   */
  template <typename T>
  decltype(auto) get_if() const {
    return std::get_if<T>(&var_);
  }

  operator bool() const { return var_.index() != 0; }

 private:
  using variant_t =
      variant<system_error, das::set_frequency, das::set_reset, das::set_config,
              das::get_config, das::analog_send_data, das::serial_send_data,
              das::digital_send_data, das::ack, das::handshake>;

  variant_t var_;
};

}  // namespace vnigma

#endif  // VNIGMA_MESSAGE_VARIANT_H
