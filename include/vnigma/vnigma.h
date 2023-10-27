#pragma once

#include <cstdint>

#include <vnigma/export.h>
#include <vnigma/target.h>
#include <vnigma/types.h>

namespace vnigma {
namespace core {
class VNIGMA_EXPORT Module {
 public:
  uint8_t id;
  Target target;
  Type type;

 public:
  explicit Module(uint8_t a_id, Target a_target, Type a_type);
};
}  // namespace core
}  // namespace vnigma
