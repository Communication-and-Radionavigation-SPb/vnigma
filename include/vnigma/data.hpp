#if !defined(VNIGMA_DATA_HPP)
#define VNIGMA_DATA_HPP
/* ----------------------------------- STD ---------------------------------- */
#include <algorithm>
#include <iostream>
#include <string>
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

namespace analog {
/* ----------------------------- Analog data ----------------------------- */

/**
 * @brief Analog response data
 * 
 * @tparam Quantity Amount of valuable fields which represents port values
 */
template <std::size_t Quantity>
class base_data {
  static_assert(Quantity != 0);

 public:
  using container_t = std::vector<int>;
  using value_type = container_t::value_type;

  using iterator = container_t::iterator;
  using const_iterator = container_t::iterator;

  using reverse_iterator = container_t::reverse_iterator;
  using const_reverse_iterator = container_t::const_reverse_iterator;

  using reference = container_t::reference;
  using const_reference = container_t::const_reference;

  using pointer = container_t::pointer;
  using const_pointer = container_t::const_pointer;

  using size_type = container_t::size_type;

  static constexpr std::size_t quantity = Quantity;

 private:
  container_t data_;

 public:
  base_data(buffer buf) : data_{} {
    if (buf.size() == 0) {
      throw make_error(errc::bad_message, "");
    }
    buffer::size_type lpos = 0;
    buffer::size_type rpos = buffer::npos;
    container_t values;

    while (lpos < buf.size()) {
      rpos = std::min(buf.find_first_of(',', lpos), buf.size());
      uint64_t value;
      try {
        value = util::toInteger(buf.substr(lpos, rpos - lpos));
      } catch (const std::exception& e) {
        throw make_error(
            errc::bad_message,
            "vnigma::analog::base_data(buffer) invalid analog data contents");
      }

      values.push_back(value);
      lpos = rpos + 1;
    }

    if (values.size() != quantity) {
      throw make_error(
          errc::bad_message,
          "vnigma::analog::base_data(buffer) invalid analog data quantity");
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

 public:
  size_type size() const noexcept { return data_.size(); }

  iterator begin() { return data_.begin(); }

  iterator end() { return data_.end(); }

  value_type at(size_type pos) { return data_.at(pos); }
};

using data = base_data<8>;
}  // namespace analog

namespace digital {
/* ----------------------------- Digital data ---------------------------- */
template <std::size_t Quantity>
class data {
 private:
  buffer payload_;
  std::vector<int8_t> values_;

 public:
  data(buffer buf) : payload_(buf), values_(Quantity) {}

 public:
  buffer value() const { return payload_; }
};
}  // namespace digital

}  // namespace vnigma

#endif  // VNIGMA_DATA_HPP
