#if !defined(VNIGMA_DATA_HPP)
#define VNIGMA_DATA_HPP
/* ----------------------------------- STD ---------------------------------- */
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>
/* -------------------------------- Internal -------------------------------- */
#include <vnigma/buffer.hpp>
#include <vnigma/exception.hpp>
#include <vnigma/util/number.hpp>
#include <vnigma/util/parameter_pack.hpp>

/* -------------------------------------------------------------------------- */
/*                                   Payload                                  */
/* -------------------------------------------------------------------------- */
namespace vnigma {
namespace serial {
/* ----------------------------- Serial data ----------------------------- */
class data {
 private:
  buffer payload_;

 public:
  data(buffer buf) : payload_(buf) {}

 public:
  buffer value() const { return payload_; }
};
}  // namespace serial

namespace base {
template <typename value_type, std::size_t Quantity,
          typename = std::enable_if_t<std::is_same_v<value_type, int> ||
                                      std::is_same_v<value_type, bool>>>
class base_data {
  static_assert(Quantity != 0);

 public:
  using container_t = std::vector<value_type>;

  using iterator = typename container_t::iterator;
  using const_iterator = typename container_t::iterator;

  using reverse_iterator = typename container_t::reverse_iterator;
  using const_reverse_iterator = typename container_t::const_reverse_iterator;

  using reference = typename container_t::reference;
  using const_reference = typename container_t::const_reference;

  using pointer = typename container_t::pointer;
  using const_pointer = typename container_t::const_pointer;

  using size_type = typename container_t::size_type;

  static constexpr std::size_t quantity = Quantity;

 private:
  container_t data_;

 public:
  base_data(buffer buf) : data_{} {
    if (buf.size() == 0) {
      throw make_error(errc::bad_message, "");
    }
    container_t values;

    extract(values, buf);

    if (values.size() != quantity) {

      std::stringstream ss;
      ss << "vnigma::base_data<" << quantity << "," << typeid(value_type).name()
         << ">"
         << "invalid data quantity";
      throw make_error(errc::bad_message, ss.str());
    }
    std::cout << "[result] ";
    for (auto&& i : values) {
      std::cout << i << " ";
    }
    std::cout << std::endl;

    data_ = values;
  }

  template <
      typename... Ts,
      typename = std::enable_if_t<
          std::conjunction<std::is_convertible<value_type, Ts>...>::value &&
          length<Ts...>::value == quantity>>
  explicit base_data(Ts... args) : data_{args...} {}

  explicit base_data(const container_t& v) : data_(v) {
    assert(v.size() == quantity);
  }

 private:
  template <typename = std::enable_if_t<std::is_same_v<value_type, int>>>
  void extract(container_t& values, buffer& buf) {
    buffer::size_type lpos = 0;
    buffer::size_type rpos = buffer::npos;
    while (lpos < buf.size()) {
      rpos = std::min(buf.find_first_of(',', lpos), buf.size());
      uint64_t value;
      try {
        value = util::toInteger(buf.substr(lpos, rpos - lpos));
        values.push_back(value);
      } catch (const std::exception& e) {
        std::stringstream ss;
        ss << "vnigma::base_data<" << quantity << ","
           << typeid(value_type).name() << ">"
           << "invalid contents data";
        throw make_error(errc::bad_message, ss.str());
      }

      lpos = rpos + 1;
    }
  }

 public:
  size_type size() const noexcept { return data_.size(); }

  iterator begin() { return data_.begin(); }

  iterator end() { return data_.end(); }

  value_type at(size_type pos) { return data_.at(pos); }
};
}  // namespace base

namespace analog {
/* ----------------------------- Analog data ----------------------------- */

/**
 * @brief Analog response data
 * 
 * @tparam Quantity Amount of valuable fields which represents port values
 */

using data = base::base_data<int, 8>;
}  // namespace analog

namespace digital {
/* ----------------------------- Digital data ---------------------------- */
/**
 * @brief Digital ports data representation
 */
using data = base::base_data<bool, 16>;
}  // namespace digital

}  // namespace vnigma

#endif  // VNIGMA_DATA_HPP
