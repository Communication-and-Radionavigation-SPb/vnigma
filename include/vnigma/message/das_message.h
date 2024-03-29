#pragma once

#include <vnigma/export.h>
#include <vnigma/id_t.h>
#include <vnigma/buffer.hpp>
#include <vnigma/exception.hpp>

#if !defined(VNIGMA_MESSAGE_DAS_MESSAGE_HPP)
#define VNIGMA_MESSAGE_DAS_MESSAGE_HPP

namespace vnigma { namespace das {
class VNIGMA_EXPORT base {
 private:
  uuid id_;

 protected:
  base(uuid id);
  base(buffer& buf);

 public:
  uuid get_uuid() const;

 private:
  [[noreturn]] void error(errc ec, const std::string& error);
};
}}  // namespace vnigma::das

#endif  // VNIGMA_MESSAGE_DAS_MESSAGE_HPP
