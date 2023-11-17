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

Type type_from_string(const char ch) {
  if (ch == 'A')
    return Type::analog;
  if (ch == 'D')
    return Type::digital;
  if (ch == 'S')
    return Type::serial;

  std::stringstream ss;
  ss << "could not extract any known module type from `" << ch << "` value";
  throw std::invalid_argument(ss.str());
}

}}  // namespace vnigma::core
