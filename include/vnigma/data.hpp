#if !defined(VNIGMA_DATA_HPP)
#define VNIGMA_DATA_HPP
/* ----------------------------------- STD ---------------------------------- */
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <typeinfo>
#include <vector>
/* -------------------------------- Internal -------------------------------- */
#include <vnigma/util/variant.h>
#include <vnigma/buffer.hpp>
#include <vnigma/exception.hpp>
#include <vnigma/util/number.hpp>
#include <vnigma/util/parameter_pack.hpp>

/* -------------------------------------------------------------------------- */
/*                                   Payload                                  */
/* -------------------------------------------------------------------------- */
namespace vnigma {

namespace base {
template <typename value_type, std::size_t Quantity,
          typename = std::enable_if_t<std::is_arithmetic_v<value_type>>>
class quntified_data {
  static_assert(Quantity != 0);

 public:
  using item_type = value_type;
  using container_t = std::vector<item_type>;

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
  quntified_data(buffer buf) : data_{} {
    if (buf.size() == 0) {
      throw make_error(errc::bad_message, "");
    }
    container_t values;
    /* ----------------------------- extract values ----------------------------- */
    buffer::size_type lpos = 0;
    buffer::size_type rpos = buffer::npos;
    while (lpos < buf.size()) {
      rpos = std::min(buf.find_first_of(',', lpos), buf.size());
      try {
        extract(values, (std::string_view)buf.substr(lpos, rpos - lpos));
      } catch (const system_error& e) {
        throw;
      } catch (const std::exception& e) {
        error(errc::bad_message, "invalid contents");
      }

      lpos = rpos + 1;
    }
    /* ---------------------- check validity of values size --------------------- */
    if (values.size() != quantity) {
      error(errc::bad_message, "invalid data quantity");
    }

    /* ------------------------------- update data ------------------------------ */
    data_ = values;
  }

  template <
      typename... Ts,
      typename = std::enable_if_t<
          std::conjunction<std::is_convertible<value_type, Ts>...>::value &&
          length<Ts...>::value == quantity>>
  explicit quntified_data(Ts... args) : data_{args...} {}

  explicit quntified_data(const container_t& v) : data_(v) {
    assert(v.size() == quantity);
  }

 private:
  [[noreturn]] void error(errc code, const char* err) {
    std::stringstream ss;
    std::cout << typeid(value_type).name() << std::endl;
    ss << "vnigma::quntified_data<" << quantity << ", "
       << typeid(value_type).name() << "> " << err;
    throw make_error(code, ss.str());
  }

  void extract(container_t& values, std::string_view view) {
    if constexpr (std::is_same_v<value_type, bool>) {
      if (view.size() != (quantity / 4)) {
        error(errc::bad_message, "invalid size of ports states payload");
      }
    }
    uint16_t pack;
    if constexpr (std::is_same_v<value_type, bool>) {
      pack = util::toInteger(view, 16);
    }
    else {
      pack = util::toInteger(view, 10);
      values.push_back(pack);
      return;
    }

    uint16_t mask = 0x1;
    for (size_t i = 0; i < quantity; i++) {
      bool value = pack & mask;
      values.push_back(value);
      mask = mask << 1;
    }
  }

 public:
  size_type size() const noexcept { return data_.size(); }

  iterator begin() { return data_.begin(); }

  iterator end() { return data_.end(); }

  reference at(size_type pos) { return data_.at(pos); }

  std::vector<item_type> items() const {
    return data_;
  }

  const_reference at(size_type pos) const { return data_.at(pos); }

  operator std::string() const {
    std::stringstream ss;

    size_type size = data_.size();
    if constexpr (std::is_same_v<value_type, bool>) {
      uint16_t compose = 0;
      for (size_t i = 0; i < size; i++) {
        compose ^= data_.at(i) << i;
      }
      ss << std::hex << std::uppercase << compose;
    }
    else {
      for (size_t i = 0; i < size; i++) {
        ss << std::setfill('0') << std::setw(4) << data_.at(i);
        if (i != (size - 1)) {
          ss << ",";
        }
      }
    }

    return ss.str();
  }
};
}  // namespace base

namespace analog {
/* ----------------------------- Analog data ----------------------------- */

/**
 * @brief Analog response data
 * 
 * @tparam Quantity Amount of valuable fields which represents port values
 */
using data = base::quntified_data<int, 8>;
}  // namespace analog

namespace digital {
/* ----------------------------- Digital data ---------------------------- */
/**
 * @brief Digital ports data representation
 */
using data = base::quntified_data<bool, 16>;
}  // namespace digital

namespace serial {
/* ----------------------------- Serial data ----------------------------- */
class data {
 private:
  buffer payload_;

 public:
  data(buffer buf) : payload_(buf) {}

 public:
  buffer value() const { return payload_; }

  operator std::string() const {
    std::stringstream ss;
    ss << payload_;
    return ss.str();
  }
};
}  // namespace serial

/* ------------------------------ Data variant ------------------------------ */
class data_variant {
 public:
  template <typename Data,
            std::enable_if_t<!std::is_same_v<std::decay_t<Data>, data_variant>,
                             std::nullptr_t>* = nullptr>
  data_variant(Data&& dt) : var_(std::forward<Data>(dt)) {}

  template <typename Func>
  auto visit(Func&& func) const& {
    return vnigma::visit(std::forward<Func>(func), var_);
  }

  template <typename Func>
  auto visit(Func&& func) & {
    return vnigma::visit(std::forward<Func>(func), var_);
  }

  template <typename Func>
  auto visit(Func&& func) && {
    return vnigma::visit(std::forward<Func>(func), var_);
  }

  template <typename T>
  decltype(auto) get() {
    return std::get<T>(var_);
  }

  template <typename T>
  decltype(auto) get() const {
    return std::get<T>(var_);
  }

  template <typename T>
  decltype(auto) get_if() {
    return std::get_if<T>(&var_);
  }

  template <typename T>
  decltype(auto) get_if() const {
    return std::get_if<T>(&var_);
  }

 private:
  using variant_t = variant<analog::data, digital::data, serial::data>;

  variant_t var_;
};

}  // namespace vnigma

#endif  // VNIGMA_DATA_HPP
