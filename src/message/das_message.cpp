#include <vnigma/message/das_message.h>
#include <sstream>
#include <vnigma/util/number.hpp>

namespace vnigma { namespace das {
base::base(uuid id) : id_(id) {}

base::base(buffer buf) : id_(0) {
  if (buf.empty()) {
    error(errc::bad_message, "can not be created from empty buffer");
  }
  auto lpos = std::min(buf.find_first_of(',') + 1, buf.size());
  auto rpos = std::max(buf.find_first_of(",", lpos), lpos);

  auto uidbuf = buf.substr(lpos, rpos - lpos);

  if (lpos == buffer::npos || rpos == buffer::npos || uidbuf.size() == 0) {
    error(errc::bad_message, "uuid part missed");
  }

  uint64_t uid;

  try {
    uid = util::toInteger(std::string{uidbuf.begin(), uidbuf.size()}, 10);
  } catch (const std::exception& e) {
    std::stringstream ss;
    ss << "failed to create uuid from `" << uidbuf << "`";
    error(errc::bad_message, ss.str());
  }

  id_ = uid;
}

uuid base::get_uuid() const {
  return id_;
}

[[noreturn]] void base::error(errc ec, const std::string& error) {
  std::stringstream ss;
  ss << "vnigma::das::base " << error;
  throw make_error(ec, ss.str());
}

}}  // namespace vnigma::das
