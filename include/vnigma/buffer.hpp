#if !defined(VNIGMA_BUFFER_H)
#define VNIGMA_BUFFER_H

#include <any>
#include <iterator>
#include <string_view>
#include <vnigma/util/iterator_traits.hpp>
#include <vnigma/util/move.hpp>
#include <vnigma/util/shared_ptr_array.hpp>

namespace vnigma {

using std::any;

class buffer {
 public:
  using iterator = std::string_view::iterator;
  using const_iterator = std::string_view::const_iterator;
  using reverse_iterator = std::string_view::reverse_iterator;
  using const_reverse_iterator = std::string_view::const_reverse_iterator;
  using reference = std::string_view::reference;
  using const_reference = std::string_view::const_reference;
  using value_type = std::string_view::value_type;
  using pointer = std::string_view::pointer;
  using const_pointer = std::string_view::const_pointer;
  using size_type = std::string_view::size_type;
  using difference_type = std::string_view::difference_type;

  static constexpr size_type npos = std::string_view::npos;

 public:
  constexpr buffer() noexcept = default;

  buffer(buffer const& other) noexcept = default;

  constexpr buffer(char const* s, std::size_t count) : view_(s, count) {}
  constexpr buffer(char const* s) : view_(s) {}

  template <typename It, typename End,
            typename std::enable_if_t<is_input_iterator<It>::value &&
                                      is_input_iterator<End>::value>* = nullptr>
  constexpr buffer(It first, End last)
      : view_{&*first, &*first + std::distance(first, last)} {}

  explicit constexpr buffer(std::string_view sv) : view_(sv) {}

  explicit buffer(std::string) = delete;

  buffer(std::string_view sv, any anchor)
      : view_{force_move(sv)}, anchor_{force_move(anchor)} {}

  buffer(char const* s, std::size_t count, any anchor)
      : view_{s, count}, anchor_{force_move(anchor)} {}

  buffer(char const* s, any anchor) : view_{s}, anchor_{force_move(anchor)} {}

  template <typename It, typename End,
            typename std::enable_if_t<is_input_iterator_v<It> &&
                                      is_input_iterator_v<End>>* = nullptr>
  buffer(It first, End last, any anchor)
      : view_{&*first, static_cast<std::size_t>(std::distance(first, last))},
        anchor_{force_move(anchor)} {}

  buffer& operator=(buffer const& buf) noexcept = default;

  constexpr const_iterator begin() const noexcept { return view_.begin(); }

  constexpr const_iterator cbegin() const noexcept { return view_.cbegin(); }

  constexpr const_iterator end() const noexcept { return view_.end(); }

  constexpr const_iterator cend() const noexcept { return view_.end(); }

  constexpr const_reverse_iterator rbegin() const noexcept {
    return view_.rbegin();
  }

  constexpr const_reverse_iterator crbegin() const noexcept {
    return view_.crbegin();
  }

  constexpr const_reverse_iterator rend() const noexcept {
    return view_.rend();
  }

  constexpr const_reverse_iterator crend() const noexcept {
    return view_.crend();
  }

  constexpr const_reference operator[](size_type pos) const {
    return view_[pos];
  }

  constexpr const_reference at(size_type pos) const { return view_.at(pos); }

  constexpr const_reference front() const { return view_.front(); }

  constexpr const_reference back() const { return view_.back(); }
  /* ---------------------------------- Data ---------------------------------- */
  constexpr const_pointer data() const noexcept { return view_.data(); }
  /* ------------------------------ Content tests ----------------------------- */
  constexpr size_type size() const noexcept { return view_.size(); }

  constexpr size_type length() const noexcept { return view_.length(); }

  constexpr size_type max_size() const noexcept { return view_.max_size(); }

  constexpr bool empty() const noexcept { return view_.empty(); }

  /* -------------------------- Remove suffix/prefix -------------------------- */

  /**
   * @brief removes specified amount of elements
   * from the begining of buffer
   * @param n amount of elements to remove
   */
  constexpr void remove_prefix(size_type n) { view_.remove_prefix(n); }

  /**
   * @brief removes specified amount of element
   * from the end of buffer
   * @param n amount of elements to remove
   */
  constexpr void remove_suffix(size_type n) { view_.remove_suffix(n); }
  /* ---------------------------------- Swap ---------------------------------- */

  /**
   * @brief swaps buffers contents
   * @param buf buffer to swap contents with
   */
  void swap(buffer& buf) noexcept {
    view_.swap(buf.view_);
    anchor_.swap(buf.anchor_);
  }

  /* ---------------------------------- Copy ---------------------------------- */

  size_type copy(char* dest, size_type count, size_type pos = 0) const {
    return view_.copy(dest, count, pos);
  }
  /* -------------------------------- Substring ------------------------------- */

  buffer substr(size_type pos = 0, size_type count = npos) const& {
    return buffer(view_.substr(pos, count), anchor_);
  }

  buffer substr(size_type pos = 0, size_type count = npos) && {
    return buffer(view_.substr(pos, count), force_move(anchor_));
  }
  /* --------------------------------- Compare -------------------------------- */

  constexpr int compare(buffer const& buf) const noexcept {
    return view_.compare(buf.view_);
  }

  constexpr int compare(std::string_view const& v) const noexcept {
    return view_.compare(v);
  }

  constexpr int compare(size_type pos, size_type count,
                        buffer const& buf) const noexcept {
    return view_.compare(pos, count, buf.view_);
  }

  constexpr int compare(size_type pos, size_type count,
                        std::string_view const& v) const noexcept {
    return view_.compare(pos, count, v);
  }

  constexpr int compare(size_type pos1, size_type count1, buffer const& buf,
                        size_type pos2, size_type count2) const noexcept {
    return view_.compare(pos1, count1, buf.view_, pos2, count2);
  }

  constexpr int compare(size_type pos1, size_type count1,
                        std::string_view const& v, size_type pos2,
                        size_type count2) const noexcept {
    return view_.compare(pos1, count1, v, pos2, count2);
  }

  constexpr int compare(char const* s) const noexcept {
    return view_.compare(s);
  }

  constexpr int compare(size_type pos1, size_type count1,
                        char const* s) const noexcept {
    return view_.compare(pos1, count1, s);
  }

  constexpr int compare(size_type pos1, size_type count1, char const* s,
                        size_type pos2, size_type count2) const noexcept {
    return view_.compare(pos1, count1, s, pos2, count2);
  }
  /* ---------------------------------- Find ---------------------------------- */
  constexpr size_type find(buffer const& buf,
                           size_type pos = 0) const noexcept {
    return view_.find(buf.view_, pos);
  }

  constexpr size_type find(std::string_view const& sv,
                           size_type pos) const noexcept {
    return view_.find(sv, pos);
  }

  constexpr size_type find(char ch, size_type pos = 0) const noexcept {
    return view_.find(ch, pos);
  }

  constexpr size_type find(char const* s, size_type pos,
                           size_type count) const {
    return view_.find(s, pos, count);
  }

  constexpr size_type find(char const* s, size_type pos = 0) const {
    return view_.find(s, pos);
  }

  /* ---------------------------------- RFind --------------------------------- */
  constexpr size_type rfind(buffer const& buf,
                            size_type pos = npos) const noexcept {
    return view_.rfind(buf.view_, pos);
  }

  constexpr size_type rfind(std::string_view const& sv,
                            size_type pos = npos) const noexcept {
    return view_.rfind(sv, pos);
  }

  constexpr size_type rfind(char ch, size_type pos = npos) const noexcept {
    return view_.rfind(ch, pos);
  }

  constexpr size_type rfind(char const* s, size_type pos,
                            size_type count) const {
    return view_.rfind(s, pos, count);
  }

  constexpr size_type rfind(char const* s, size_type pos = npos) const {
    return view_.rfind(s, pos);
  }
  /* ------------------------------- Find first ------------------------------- */
  constexpr size_type find_first_of(buffer const& buf,
                                    size_type pos = 0) const noexcept {
    return view_.find_first_of(buf.view_, pos);
  }

  constexpr size_type find_first_of(std::string_view v,
                                    size_type pos = 0) const noexcept {
    return view_.find_first_of(v, pos);
  }

  constexpr size_type find_first_of(char ch, size_type pos = 0) const noexcept {
    return view_.find_first_of(ch, pos);
  }

  constexpr size_type find_first_of(char const* s, size_type pos,
                                    size_type count) const {
    return view_.find_first_of(s, pos, count);
  }

  constexpr size_type find_first_of(char const* s, size_type pos = 0) const {
    return view_.find_first_of(s, pos);
  }

  /* -------------------------------- Find last ------------------------------- */
  constexpr size_type find_last_of(buffer const& buf,
                                   size_type pos = npos) const noexcept {
    return view_.find_last_of(buf.view_, pos);
  }

  constexpr size_type find_last_of(std::string_view v,
                                   size_type pos = npos) const noexcept {
    return view_.find_last_of(v, pos);
  }

  constexpr size_type find_last_of(char ch,
                                   size_type pos = npos) const noexcept {
    return view_.find_last_of(ch, pos);
  }

  constexpr size_type find_last_of(char const* s, size_type pos,
                                   size_type count) const {
    return view_.find_last_of(s, pos, count);
  }

  constexpr size_type find_last_of(char const* s, size_type pos = npos) const {
    return view_.find_last_of(s, pos);
  }
  /* ----------------------------- Find first not ----------------------------- */
  constexpr size_type find_first_not_of(buffer const& buf,
                                        size_type pos = 0) const noexcept {
    return view_.find_first_not_of(buf.view_, pos);
  }

  constexpr size_type find_first_not_of(std::string_view v,
                                        size_type pos = 0) const noexcept {
    return view_.find_first_not_of(v, pos);
  }

  constexpr size_type find_first_not_of(char ch,
                                        size_type pos = 0) const noexcept {
    return view_.find_first_not_of(ch, pos);
  }

  constexpr size_type find_first_not_of(char const* s, size_type pos,
                                        size_type count) const {
    return view_.find_first_not_of(s, pos, count);
  }

  constexpr size_type find_first_not_of(char const* s,
                                        size_type pos = 0) const {
    return view_.find_first_not_of(s, pos);
  }
  /* ----------------------------- Find last not ----------------------------- */
  constexpr size_type find_last_not_of(buffer const& buf,
                                       size_type pos = npos) const noexcept {
    return view_.find_last_not_of(buf.view_, pos);
  }

  constexpr size_type find_last_not_of(std::string_view v,
                                       size_type pos = npos) const noexcept {
    return view_.find_last_not_of(v, pos);
  }

  constexpr size_type find_last_not_of(char ch,
                                       size_type pos = npos) const noexcept {
    return view_.find_last_not_of(ch, pos);
  }

  constexpr size_type find_last_not_of(char const* s, size_type pos,
                                       size_type count) const {
    return view_.find_last_not_of(s, pos, count);
  }

  constexpr size_type find_last_not_of(char const* s,
                                       size_type pos = npos) const {
    return view_.find_last_not_of(s, pos);
  }
  /* ------------------------------- Conversion ------------------------------- */
  operator std::string_view() const { return view_; }
  /* ------------------------------- Comparison ------------------------------- */
  friend constexpr bool operator==(buffer const& lhs,
                                   buffer const& rhs) noexcept {
    return lhs.view_ == rhs.view_;
  }

  friend constexpr bool operator!=(buffer const& lhs,
                                   buffer const& rhs) noexcept {
    return lhs.view_ != rhs.view_;
  }

  friend constexpr bool operator<(buffer const& lhs,
                                  buffer const& rhs) noexcept {
    return lhs.view_ < rhs.view_;
  }

  friend constexpr bool operator<=(buffer const& lhs,
                                   buffer const& rhs) noexcept {
    return lhs.view_ <= rhs.view_;
  }

  friend constexpr bool operator>(buffer const& lhs,
                                  buffer const& rhs) noexcept {
    return lhs.view_ > rhs.view_;
  }

  friend constexpr bool operator>=(buffer const& lhs,
                                   buffer const& rhs) noexcept {
    return lhs.view_ >= rhs.view_;
  }

  friend std::ostream& operator<<(std::ostream& out, buffer const& buf) {
    out << buf.view_;
    return out;
  }

  any const& get_anchor() const { return anchor_; }

 private:
  std::string_view view_;
  any anchor_;
};

inline namespace literals {
inline buffer operator""_mb(char const* str, std::size_t length) {
  return buffer(str, length);
}
}  // namespace literals

template <typename Iterator>
inline buffer allocate_buffer(Iterator b, Iterator e) {
  auto size = static_cast<std::size_t>(std::distance(b, e));
  if (size == 0)
    return buffer(&*b, size);
  auto spa = make_shared_ptr_array(size);
  std::copy(b, e, spa.get());
  auto p = spa.get();
  return buffer(p, size, force_move(spa));
}

inline buffer allocate_buffer(std::string_view sv) {
  return allocate_buffer(sv.begin(), sv.end());
}

}  // namespace vnigma

#endif  // VNIGMA_BUFFER_H
