#include <vnigma/target.h>
#include <sstream>
#include <stdexcept>

namespace vnigma { namespace core {
Target target_from_string(const std::string& str) {

  if(str == "DS") return Target::das;
  if(str == "VN") return Target::vn;

  std::stringstream ss;
  ss << "Could not resolve target device from " << str;
  throw std::invalid_argument(ss.str());
}

std::string target_to_string(const Target& tgt) noexcept {
  if (Target::das == tgt)
    return "DS";
  if (Target::vn == tgt)
    return "VN";
  return "";
}

}}  // namespace vnigma::core
