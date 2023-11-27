#include <gtest/gtest.h>
/* ----------------------------------- STD ---------------------------------- */
#include <optional>
#include <string>
/* --------------------------------- Vnigma --------------------------------- */
#include <vnigma/message/das_get_config.h>
#include <vnigma/buffer.hpp>
#include <vnigma/message/message_traits.hpp>
#include <vnigma/message/message_variant.hpp>

#define Suite GetConfigTest

namespace vn {
using namespace vnigma;
using namespace vnigma::das;
};  // namespace vn
using std::optional;
using std::string;
using namespace vn::literals;

struct das_gc_p {
  vn::uuid uid;
  vn::buffer buf;
  vn::device dev;
  optional<errc> code;
  optional<std::string> error = std::nullopt;
};

class GetConfigTest : public ::testing::TestWithParam<das_gc_p> {};

TEST_F(Suite, traits) {
  EXPECT_TRUE(vn::is_command<vn::get_config>());
  EXPECT_FALSE(vn::is_response<vn::get_config>());

  EXPECT_TRUE(vn::das_related<vn::get_config>());
  EXPECT_FALSE(vn::venom_related<vn::get_config>());

  EXPECT_FALSE(vn::has_payload<vn::get_config>());

  EXPECT_FALSE(vn::is_port_missed<vn::get_config>());
  EXPECT_FALSE(vn::is_port_scoped<vn::get_config>());
}

TEST_P(Suite, as_buffer) {
  auto param = GetParam();
  if (param.code.has_value()) {
    GTEST_SKIP() << "Skip cause should fail";
  }
  vn::get_config msg(param.uid, param.dev);

  auto buf = msg.as_buffer();
  EXPECT_EQ(buf, param.buf) << buf << " is not equal to " << param.buf;
}

TEST_P(Suite, from_buffer) {
  auto param = GetParam();
  if (param.code.has_value()) {
    GTEST_SKIP() << "Skip cause should fail";
  }
  std::cout << "[debug]: " << param.buf << std::endl;
  vn::get_config msg(param.buf);

  vn::buffer buf = msg.as_buffer();
  EXPECT_EQ(buf, param.buf) << buf << " is not equal to " << param.buf;
}

INSTANTIATE_TEST_SUITE_P(  // instantiate test suite
    GetConfig, GetConfigTest,
    ::testing::Values(  // values
        das_gc_p{100, "<DSSGC,100,9\r\n"_mb, vn::device(9, vn::core::serial),
                 std::nullopt}  // end
        )                       // values
);