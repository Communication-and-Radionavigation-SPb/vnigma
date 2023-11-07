#pragma once

#include <vnigma/export.h>

namespace vnigma { namespace core {
class VNIGMA_EXPORT GuidGenerator {
 public:
  virtual unsigned int next() const noexcept;
};
}}  // namespace vnigma::core
