#include <vnigma/digital/types.h>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>

namespace vnigma { namespace digital { namespace data {

std::string to_string(const Period& period) {
  switch (period) {
    case never:
      return "-1";
    case fastest:
      return "500";
    case fast:
      return "1000";
    case normal:
      return "2000";
    case slow:
      return "5000";
    case slower:
      return "10000";
    default:
      throw std::runtime_error(
          "Unimplemented digital data period string conversion.");
  }
}

Period from_string(const std::string& str) {
  if (std::any_of(str.begin(), str.end(),
                  [](const char& c) { return !std::isdigit(c) && c != '-'; })) {
    std::stringstream ss;
    ss << "Digital data transmission period value can contain only digits, but "
          "`"
       << str << "`"
       << " passed";
    throw std::runtime_error(ss.str());
  }

  int16_t value = std::atoi(str.c_str());
  if (value == never)
    return never;
  if (value == fastest)
    return fastest;
  if (value == fast)
    return fast;
  if (value == normal)
    return normal;
  if (value == slow)
    return slow;
  if (value == slower)
    return slower;

  std::stringstream ss;
  ss << "Could not decode digital data transmission period from `" << str
     << "`";
  throw std::runtime_error(ss.str());
}

}}}  // namespace vnigma::digital::data
