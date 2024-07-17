#if !defined(VNIGMA_UTIL_BUFFER_MANIP)
#define VNIGMA_UTIL_BUFFER_MANIP

#include <vnigma/buffer.hpp>
#include <vnigma/exception.hpp>
namespace vnigma {

void print_buffer(buffer buf);

void debug_found_pos(buffer& buf, buffer::size_type pos);

buffer skip(buffer& buf, size_t count);

/// @brief Does nothing after termination symbols changed
[[deprecated]] inline buffer trim_buffer(buffer& buf) {
  // return buf.substr(0, buf.size() - 2);
  // Due to changes in protocol termination symobls
  // replaced with '\0', so this function should do nothing
  return buf;
}

}  // namespace vnigma

#endif  // VNIGMA_UTIL_BUFFER_MANIP
