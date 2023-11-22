#if !defined(VNIGMA_UTIL_NUMBER_HPP)
#define VNIGMA_UTIL_NUMBER_HPP

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace vnigma { namespace util {
/**
   * @brief converts string_view to double.
   * 
   * @throws when passed string_view contains invalid characters
   *         or empty 
   * @param s string_view to convert
   * @return double converted string_view
   */
inline double toDouble(const std::string_view& s) {
  if (s.empty()) {
    std::stringstream ss;
    ss << "vnigma::util::toDouble can not make double from empty source";
    throw std::invalid_argument(ss.str());
  }

  char* p;
  double result = std::strtod(s.data(), &p);
  if (*p != 0) {
    std::stringstream ss;
    ss << "vnigma::util::toDouble could not convert '" << s
       << "' to double, error at '" << *p << "'";
    throw std::invalid_argument(ss.str());
  }

  return result;
}

inline int64_t toInteger(const std::string_view& sv, int radix = 10) {
  if (sv.empty()) {
    std::stringstream ss;
    ss << "vnigma::util::toInteger can not make integer from empty source";
    throw std::invalid_argument(ss.str());
  }

  char* p;
  std::string s{sv.begin(), sv.size()};
  int64_t d = std::strtoll(s.data(), &p, radix);

  if (*p != 0) {
    std::stringstream ss;
    ss << "vnigma::util::toInteger could not convert '" << s
       << "' to integer, error at '" << *p << "'";
    throw std::invalid_argument(ss.str());
  }

  return d;
}

inline bool isInteger(const std::string_view& s) {
  if (s.empty())
    return false;

  auto it = s.begin();
  while (it != s.end()) {
    if (!std::isdigit(*it) && *it != '-')
      return false;
    it++;
  }
  return true;
}

}}  // namespace vnigma::util

#endif  // VNIGMA_UTIL_NUMBER_HPP
