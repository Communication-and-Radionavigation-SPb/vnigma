#include <vnigma/types.h>
#include <sstream>

namespace vnigma { namespace core {

std::string type_to_string(const Type& t) noexcept {
  if (t == Type::analog)
    return "A";
  if (t == Type::digital)
    return "D";
  if (t == Type::serial)
    return "S";
  return "U";
}

Type type_from_string(const std::string& str) {

  if (str.length() == 1) {
    if (str == "A")
      return Type::analog;
    if (str == "D")
      return Type::digital;
    if (str == "S")
      return Type::serial;
  }

  std::stringstream ss;
  ss << "Could not extract any known module type from `" << str << "` value";
  throw std::invalid_argument(ss.str());
}
}}  // namespace vnigma::core
