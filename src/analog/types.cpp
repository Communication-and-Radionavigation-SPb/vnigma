#include <vnigma/analog/types.h>
#include <sstream>
#include <stdexcept>

namespace vnigma { namespace analog {
namespace data {
std::string to_string(const Period& d) {
  switch (d) {
    case data::never:
      return "-1";
    case data::fastest:
      return "500";
    case data::fast:
      return "1000";
    case data::normal:
      return "2000";
    case data::slow:
      return "5000";
    case data::slower:
      return "10000";
  }
  return "-1";
}

Period from_string(const std::string& str) {
  if (str == "-1")
    return Period::never;
  if (str == "500")
    return Period::fastest;
  if (str == "1000")
    return Period::fast;
  if (str == "2000")
    return Period::normal;
  if (str == "5000")
    return Period::slow;
  if (str == "10000")
    return Period::slower;
  //  Throw if not matches
  std::stringstream ss;
  ss << "Can not decode analog module get data frequency from `" << str << "`";
  throw std::invalid_argument(ss.str());
}
}  // namespace data
namespace config {
std::string to_string(const Period& d) {
  switch (d) {
    case config::never:
      return "-1";
    case config::fastest:
      return "2";
    case config::fast:
      return "5";
    case config::slow:
      return "10";
    case config::slower:
      return "60";
  }
  return "-1";
}

Period from_string(const std::string& str) {
  if (str == "-1")
    return Period::never;
  if (str == "2")
    return Period::fastest;
  if (str == "5")
    return Period::fast;
  if (str == "10")
    return Period::slow;
  if (str == "60")
    return Period::slower;
  //  Throw if not matches
  std::stringstream ss;
  ss << "Can not decode analog module get config frequency from `" << str
     << "`";
  throw std::invalid_argument(ss.str());
}
}  // namespace config

}}  // namespace vnigma::analog
