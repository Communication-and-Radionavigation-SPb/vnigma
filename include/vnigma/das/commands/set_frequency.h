#pragma once

#include <vnigma/export.h>
#include <vnigma/vnigma.h>

#include <string>

namespace vnigma { namespace das { namespace commands {
class VNIGMA_EXPORT SetFrequency {
 private:
  vnigma::core::Module& module_;

 public:
  SetFrequency(const vnigma::core::Module& target);

 public:
  operator std::string() const { return ""; }
};
}}}  // namespace vnigma::das::commands
