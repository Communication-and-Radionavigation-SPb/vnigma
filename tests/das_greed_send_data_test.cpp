#include <gtest/gtest.h>
#include <vnigma/message/das_greed_send_data.h>
#include <vnigma/buffer.hpp>
#include <vnigma/message/message_traits.hpp>

#include "das_sd_p.h"
#include "mock_devices.hpp"

namespace vn {
using namespace vnigma;
using vnigma::das::greed_send_data;
}  // namespace vn

using std::optional;
using std::tuple;
using std::variant;
using namespace vnigma::literals;

class GreedSendDataTest : public ::testing::TestWithParam<das_sd_p> {};

TEST_F(GreedSendDataTest, traits) {
  EXPECT_FALSE(vn::is_command<vn::greed_send_data>());
  EXPECT_TRUE(vn::is_response<vn::greed_send_data>());

  EXPECT_TRUE(vn::das_related<vn::greed_send_data>());
  EXPECT_FALSE(vn::venom_related<vn::greed_send_data>());

  EXPECT_TRUE(vn::is_port_missed<vn::greed_send_data>());
  EXPECT_FALSE(vn::is_port_scoped<vn::greed_send_data>());

  EXPECT_TRUE(vn::has_payload<vn::greed_send_data>());

  EXPECT_TRUE(vn::is_data<vn::greed_send_data>());
}

TEST_P(GreedSendDataTest, as_buffer) {
  auto param = GetParam();

  vn::greed_send_data cmd(param.dev, param.data);

  EXPECT_EQ(cmd.as_buffer(), param.buf)
      << cmd.as_buffer() << " is not equal to " << param.buf;
}

TEST_P(GreedSendDataTest, from_buffer) {
  auto param = GetParam();

  vn::greed_send_data cmd(param.buf);

  EXPECT_EQ(cmd.as_buffer(), param.buf)
      << cmd.as_buffer() << "not equal to " << param.buf;
}

INSTANTIATE_TEST_SUITE_P(
    DasSD, GreedSendDataTest,
    ::testing::Values(
        das_sd_p{"<DSASD,1,,1506,0000,3279,1235,0194,2527,1215,2396\r\n"_mb,
                 mock::f_analog(),
                 vn::analog::data(1506, 0, 3279, 1235, 194, 2527, 1215, 2396)},
        das_sd_p{"<DSDSD,1,,FFFF\r\n"_mb, mock::f_digital(),
                 vn::digital::data(true, true, true, true, true, true, true,
                                   true, true, true, true, true, true, true,
                                   true, true)}));
