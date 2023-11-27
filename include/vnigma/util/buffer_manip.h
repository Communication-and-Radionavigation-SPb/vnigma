#if !defined(VNIGMA_UTIL_BUFFER_MANIP)
#define VNIGMA_UTIL_BUFFER_MANIP

#include <algorithm>
#include <vnigma/buffer.hpp>
#include <vnigma/exception.hpp>
namespace vnigma {

void print_buffer(buffer buf);

void debug_found_pos(buffer& buf, buffer::size_type pos);

buffer skip(buffer& buf, size_t count);

inline buffer trim_buffer(buffer& buf) {
  return buf.substr(0, buf.size() - 2);
}

}  // namespace vnigma

#endif  // VNIGMA_UTIL_BUFFER_MANIP
