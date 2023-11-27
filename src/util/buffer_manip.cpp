#include <vnigma/util/buffer_manip.h>
#include <iomanip>
#include <iostream>

namespace vnigma {

buffer skip(buffer& buf, size_t count) {
  buffer::size_type pos = 0;
  for (size_t i = 0; i < count; i++) {
    pos = buf.find_first_of(',', pos) + 1;
    if (pos >= buf.size() - 1)
      break;
  }
  return buf.substr(std::min(pos, buf.size()));
}

void print_buffer(buffer buf) {
  std::cout << buf << std::endl;
}

void debug_found_pos(buffer& buf, buffer::size_type pos) {
  auto corrected_pos = std::min(pos, buf.size() + 1);
  std::cout << buf << std::endl;
  std::cout << std::setw(corrected_pos) << std::setfill(' ') << "^"
            << std::endl;
}

}  // namespace vnigma
