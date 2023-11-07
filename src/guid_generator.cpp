#include <vnigma/guid_generator.h>

namespace vnigma { namespace core {

unsigned int GuidGenerator::next() const noexcept {
  static unsigned int current = 1;
  return current++;
}

}}  // namespace vnigma::core
