#include <vnigma/vnigma.h>

namespace vnigma { namespace core {
Module::Module(uint8_t a_id, Target a_target, Type a_type)
    : id_(a_id), target_(a_target), type_(a_type) {}

std::string Module::prefix() const noexcept {
  return target_to_string(target_) + type_to_string(type_);
}

Target Module::target() const noexcept {
  return target_;
}

Type Module::type() const noexcept {
  return type_;
}

uint8_t Module::id() const noexcept {
  return id_;
}

}}  // namespace vnigma::core
