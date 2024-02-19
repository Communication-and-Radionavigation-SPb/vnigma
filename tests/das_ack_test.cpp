#include <gtest/gtest.h>
#include <vnigma/message/das_ack.h>
#include <vnigma/buffer.hpp>
#include <vnigma/message/message_traits.hpp>

#include "mock_devices.hpp"

namespace vn {
using namespace vnigma;
using vnigma::das::ack;
}  // namespace vn

using std::optional;
using std::tuple;
using std::variant;
using namespace vnigma::literals;

struct das_ack_test {
  vn::buffer buf;
  vn::device dev;
  vn::uuid id;
  vn::buffer payload;
};

class AckScopedTests : public ::testing::TestWithParam<das_ack_test> {};

TEST_F(AckScopedTests, traits) {
  EXPECT_TRUE(vn::is_command<vn::ack>());
  EXPECT_FALSE(vn::is_response<vn::ack>());

  EXPECT_TRUE(vn::das_related<vn::ack>());
  EXPECT_FALSE(vn::venom_related<vn::ack>());

  EXPECT_TRUE(vn::is_port_missed<vn::ack>());
  EXPECT_FALSE(vn::is_port_scoped<vn::ack>());

  EXPECT_TRUE(vn::has_payload<vn::ack>());
  EXPECT_FALSE(vn::is_data<vn::ack>());
}

TEST_P(AckScopedTests, as_buffer) {
  auto param = GetParam();

  vn::ack cmd(param.id, param.dev, param.payload);

  EXPECT_EQ(cmd.as_buffer(), param.buf)
      << cmd.as_buffer() << " is not equal to " << param.buf;
}

TEST_P(AckScopedTests, from_buffer) {
  auto param = GetParam();

  vn::ack cmd(param.buf);
  auto buf = cmd.as_buffer();
  std::string payload{buf.begin(), buf.end()};
  std::string validation{param.buf.begin(), param.buf.end()};

  EXPECT_EQ(payload, validation)
      << cmd.as_buffer() << "not equal to " << param.buf;
}

INSTANTIATE_TEST_SUITE_P(DasSD, AckScopedTests,
                         ::testing::Values(das_ack_test{
                             "<DSAAK,10004,1,,WRONG_DATA\r\n"_mb,
                             mock::f_analog(),
                             10004,
                             "WRONG_DATA"_mb,
                         }));
