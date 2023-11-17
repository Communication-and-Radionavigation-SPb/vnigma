#if !defined(VNIGMA_EXCEPTION_HPP)
#define VNIGMA_EXCEPTION_HPP

#include <sstream>
#include <string_view>
#include <system_error>
#include <tuple>

using error_code = std::error_code;
using errc = std::errc;

namespace vnigma {

struct system_error : std::system_error {
  using base_type = std::system_error;
  using base_type::base_type;

  /**
   * @brief Construct a new system error object
   * 
   * @param ec error code. if omit then no error
   */
  system_error(error_code const& ec = error_code()) : base_type{ec} {}

  std::string message() const { return code().message(); }

  /**
   * @brief bool operator
   * 
   * @return true if error
   * @return false  if no error
   */
  operator bool() const { return code().value() != errno; }
};

template <typename What>
inline system_error make_error(errc ec, What&& w) {
  return system_error(std::make_error_code(ec), std::forward<What>(w));
}

inline bool operator==(system_error const& lhs, system_error const& rhs) {
  return std::tuple<error_code, std::string_view>(lhs.code(), lhs.what()) ==
         std::tuple<error_code, std::string_view>(rhs.code(), rhs.message());
}

inline std::ostream& operator<<(std::ostream& o, system_error const& v) {
  o << v.what();
  return o;
}

}  // namespace vnigma

#endif  // VNIGMA_EXCEPTION_HPP
