#include <gtest/gtest.h>
/* ----------------------------------- STD ---------------------------------- */
#include <optional>
#include <string>
/* --------------------------------- Vnigma --------------------------------- */
#include <vnigma/message/das_get_config.h>
#include <vnigma/buffer.hpp>
#include <vnigma/message/message_traits.hpp>
#include <vnigma/message/message_variant.hpp>

namespace vn {
using namespace vnigma;
using namespace vnigma::das;
};  // namespace vn
using std::optional;
using std::string;
using namespace vn::literals;

struct das_gc_p {
  vn::uuid uid;
  std::string buf;
  vn::device dev;
};

class GetConfigTest : public ::testing::TestWithParam<das_gc_p> {};

TEST_F(GetConfigTest, traits) {
  EXPECT_TRUE(vn::is_command<vn::get_config>());
  EXPECT_FALSE(vn::is_response<vn::get_config>());

  EXPECT_TRUE(vn::das_related<vn::get_config>());
  EXPECT_FALSE(vn::venom_related<vn::get_config>());

  EXPECT_FALSE(vn::has_payload<vn::get_config>());

  EXPECT_FALSE(vn::is_port_missed<vn::get_config>());
  EXPECT_FALSE(vn::is_port_scoped<vn::get_config>());

  EXPECT_FALSE(vn::is_data<vn::get_config>());
}

TEST_P(GetConfigTest, as_buffer) {
  auto param = GetParam();
  vn::get_config msg(param.uid, param.dev);

  auto buf = msg.as_buffer();
  std::string actual(buf.begin(), buf.end());
  EXPECT_EQ(actual, param.buf) << buf << " is not equal to " << param.buf;
}

TEST_P(GetConfigTest, from_buffer) {
  auto param = GetParam();
  auto inbuf = vn::allocate_buffer(param.buf);
  vn::get_config msg(inbuf);

  vn::buffer buf = msg.as_buffer();
  std::string actual(buf.begin(), buf.end());
  EXPECT_EQ(actual, param.buf);
}

INSTANTIATE_TEST_SUITE_P(  // instantiate test suite
    GetConfig, GetConfigTest,
    ::testing::Values(  // values
        das_gc_p{100, "<DSSGC,100,9", vn::device(9, vn::core::serial)}  // end
        )  // values
);
