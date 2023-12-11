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
  if (t == Type::general)
    return "X";
  return "U";
}

Type type_from_char(const char ch) {
  if (ch == 'A')
    return Type::analog;
  if (ch == 'D')
    return Type::digital;
  if (ch == 'S')
    return Type::serial;
  if (ch == 'X')
    return Type::general;

  std::stringstream ss;
  ss << "could not extract any known module type from `" << ch << "` value";
  throw std::invalid_argument(ss.str());
}

}}  // namespace vnigma::core
