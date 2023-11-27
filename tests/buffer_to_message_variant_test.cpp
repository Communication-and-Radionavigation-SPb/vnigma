#include <gtest/gtest.h>

#include <vnigma/buffer_to_message_variant.h>
#include <iostream>
#include <vnigma/buffer.hpp>
#include <vnigma/exception.hpp>

namespace vn = vnigma;
using namespace vn::literals;

#define Suite BufferToMesVar

struct err_m {
  vn::buffer buf;
  errc code;
};

class BufferErrTest : public ::testing::TestWithParam<err_m> {};

TEST_P(BufferErrTest, returns_error) {
  auto param = GetParam();
  auto msg = vn::buffer_to_message_variant(param.buf);
  EXPECT_FALSE((bool)msg);
  ASSERT_NE(msg.get_if<vn::system_error>(), nullptr);
  EXPECT_EQ(msg.get<vn::system_error>().code(), param.code);
}

INSTANTIATE_TEST_SUITE_P(  // errors suite
    Errors, BufferErrTest,
    ::testing::Values(err_m{"\r\n"_mb, errc::bad_message},     // no prefix
                      err_m{"<"_mb, errc::message_size},       // size
                      err_m{"<VN"_mb, errc::message_size},     // size
                      err_m{"<VNS"_mb, errc::message_size},    // size
                      err_m{"<VNS**"_mb, errc::message_size},  // size
                      // not supported
                      err_m{"<VNS**,1"_mb, errc::not_supported}));
