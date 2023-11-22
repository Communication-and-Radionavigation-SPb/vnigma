#include <gtest/gtest.h>
#include <vnigma/message/das_send_data.h>
#include <vnigma/buffer.hpp>
#include <vnigma/message/message_traits.hpp>

#include "mock_devices.hpp"

namespace vn {
using namespace vnigma;
using vnigma::das::send_data;
}  // namespace vn

using std::optional;
using std::tuple;
using std::variant;
using namespace vnigma::literals;

struct das_sd_p {
  vn::buffer buf;
  vn::uuid uid;
  vn::device dev;
  optional<tuple<errc, const std::string&>> error;
};

class SendDataTest : public ::testing::TestWithParam<das_sd_p> {};

TEST_F(SendDataTest, traits) {
  EXPECT_FALSE(vn::is_command<vn::send_data>());
  EXPECT_TRUE(vn::is_response<vn::send_data>());

  EXPECT_TRUE(vn::das_related<vn::send_data>());
  EXPECT_FALSE(vn::venom_related<vn::send_data>());

  EXPECT_TRUE(vn::is_port_missed<vn::send_data>());
  EXPECT_FALSE(vn::is_port_scoped<vn::send_data>());

  EXPECT_TRUE(vn::has_payload<vn::send_data>());
}

TEST_P(SendDataTest, as_buffer) {
  auto param = GetParam();
  if (param.error.has_value()) {
    GTEST_SKIP() << "Skipped cause should be tested as failed";
  }
}

TEST_P(SendDataTest, from_buffer) {
  auto param = GetParam();

  if (param.error.has_value()) {
    GTEST_SKIP() << "Skipped cause should be tested as failed";
  }

  vn::send_data cmd(param.buf);

  EXPECT_EQ(cmd.get_uuid(), param.uid);
}


INSTANTIATE_TEST_SUITE_P(DasSD, SendDataTest, ::testing::Values(
  das_sd_p{"<DSASD,101,1,,"_mb, 101, mock::f_analog(), std::nullopt}
));
