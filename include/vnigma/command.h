#pragma once

#include <vnigma/export.h>
#include <vnigma/guid_generator.h>
#include <vnigma/vnigma.h>
#include <random>
#include <sstream>
#include <iomanip>
#include <string>

namespace vnigma { namespace core {
/**
 * @brief Command template
 * 
 * @details Base operations 
 * 
 * @tparam TCommand Concrete implementation of TCommand interfcae
 */
template <class TCommand>
VNIGMA_EXPORT class Command {
 protected:
  /**
   * @brief Module reference
   * @details Owned by external source
   */
  Module* module_;

  GuidGenerator* generator_;

 public:
  Command(Module* a_module, GuidGenerator* a_generator)
      : module_(a_module), generator_(a_generator) {}

 private:
  TCommand* cast() noexcept { return static_cast<TCommand*>(this); }

 public:
  /**
   * @brief Returns string representation of command
   * 
   * @return std::string Composed command in string format
   */
  std::string compose() noexcept {
    std::string payload = cast()->get_payload();
    std::stringstream ss;
    ss << "<" << module_->prefix() << cast()->get_name() << ",";
    if (module_->target() == Target::das) {
      ss << std::setw(8) << std::setfill('0') << generator_->next() << ",";
    }
    ss << (int)module_->id();

    if (payload.size() > 0) {
      ss << "," << payload;
    }

    return ss.str();
  }
};
}}  // namespace vnigma::core
