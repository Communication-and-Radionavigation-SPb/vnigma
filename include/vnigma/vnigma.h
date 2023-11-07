#pragma once

#include <cstdint>
#include <string>

#include <vnigma/export.h>
#include <vnigma/target.h>
#include <vnigma/types.h>

namespace vnigma { namespace core {
class VNIGMA_EXPORT Module {
 private:
  uint8_t id_;
  Target target_;
  Type type_;

 public:
  explicit Module(uint8_t a_id, Target a_target, Type a_type);

 public:
  /**
   * @brief Returns module prefix which depends
   * on <Module::target>
   * 
   * @return std::string Module prefix
   */
  std::string prefix() const noexcept;

  /**
   * @brief Returns target device type
   */
  Target target() const noexcept;

  /**
   * @brief Returns type of device
   */
  Type type() const noexcept;

  /**
   * @brief Returns module identity
   * 
   * @return Module identity
   */
  uint8_t id() const noexcept;
};
}}  // namespace vnigma::core
