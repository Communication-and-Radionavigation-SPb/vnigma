#include <gtest/gtest.h>
#include <vnigma/message/message_traits.hpp>
#include <vnigma/message/message_variant.hpp>

namespace vn {
using namespace vnigma;
using vnigma::das::set_reset;
}  // namespace vn

using namespace vnigma::literals;

struct das_sr_p {
  vn::uuid uid;
  vn::buffer buf;
  vn::device dev;
};

class SetResetTest : public ::testing::TestWithParam<das_sr_p> {};

TEST_F(SetResetTest, traits) {
  EXPECT_TRUE(vn::is_command<vn::set_reset>());
  EXPECT_FALSE(vn::is_response<vn::set_reset>());
  EXPECT_FALSE(vn::has_payload<vn::set_reset>());
  EXPECT_FALSE(vn::is_port_missed<vn::set_reset>());
  EXPECT_FALSE(vn::is_port_scoped<vn::set_reset>());
  EXPECT_FALSE(vn::is_data<vn::set_reset>());
}

TEST_P(SetResetTest, as_buffer) {
  auto param = GetParam();
  vn::set_reset cmd(param.uid, param.dev);

  auto buf = cmd.as_buffer();
  EXPECT_EQ(buf, param.buf) << buf << " is not equal to " << param.buf;
}

TEST_P(SetResetTest, from_buffer) {
  auto param = GetParam();

  vn::set_reset cmd(param.buf);

  auto buf = cmd.as_buffer();
  EXPECT_EQ(buf, param.buf);
}

INSTANTIATE_TEST_SUITE_P(
    DasSetReset, SetResetTest,
    ::testing::Values(
        das_sr_p{100, "<DSSSR,100,1\r\n"_mb, vn::device(1, vn::core::serial)},
        das_sr_p{100, "<DSASR,100,1\r\n"_mb, vn::device(1, vn::core::analog)},
        das_sr_p{100, "<DSDSR,100,1\r\n"_mb,
                 vn::device(1, vn::core::digital)}));
