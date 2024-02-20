#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vnigma/message/das_analog_send_data.h>
#include <vnigma/buffer.hpp>
#include <vnigma/message/message_traits.hpp>

#include "das_sd_p.h"
#include "mock_devices.hpp"

namespace vn {
using namespace vnigma;
using vnigma::das::analog_send_data;
}  // namespace vn

using std::optional;
using std::tuple;
using std::variant;
using namespace vnigma::literals;

class AnalogSendDataTest : public ::testing::TestWithParam<das_sd_p> {};

TEST_F(AnalogSendDataTest, traits) {
  EXPECT_FALSE(vn::is_command<vn::analog_send_data>());
  EXPECT_TRUE(vn::is_response<vn::analog_send_data>());

  EXPECT_TRUE(vn::das_related<vn::analog_send_data>());
  EXPECT_FALSE(vn::venom_related<vn::analog_send_data>());

  EXPECT_TRUE(vn::is_port_missed<vn::analog_send_data>());
  EXPECT_FALSE(vn::is_port_scoped<vn::analog_send_data>());

  EXPECT_TRUE(vn::has_payload<vn::analog_send_data>());

  EXPECT_TRUE(vn::is_data<vn::analog_send_data>());
}

TEST_F(AnalogSendDataTest, target_device) {
  EXPECT_ANY_THROW({ vn::analog_send_data("<DSSSD,1,,1,1,1,1,1,1,1,1\r\n"_mb); });
  EXPECT_ANY_THROW({ vn::analog_send_data("<DSDSD,1,,1,1,1,1,1,1,1,1\r\n"_mb); });
  EXPECT_NO_THROW({ vn::analog_send_data("<DSASD,1,,1,1,1,1,1,1,1,1\r\n"_mb); });
}

TEST_F(AnalogSendDataTest, correctly_resolves_buffer) {
  vn::analog_send_data cmd("<DSASD,1,,1,2,3,4,5,6,7,8\r\n"_mb);

  EXPECT_EQ(cmd.get_device().id(), 1);
  EXPECT_EQ(cmd.get_device().type(), vnigma::Type::analog);

  EXPECT_EQ(cmd.payload(), "0001,0002,0003,0004,0005,0006,0007,0008");

  std::vector<int> actual = cmd.get_data().items();

  EXPECT_THAT(actual, ::testing::SizeIs(8));
  EXPECT_THAT(actual, ::testing::ElementsAre(1, 2, 3, 4, 5, 6, 7, 8));
}

TEST_P(AnalogSendDataTest, as_buffer) {
  auto param = GetParam();

  vn::analog_send_data cmd(param.dev, param.data);

  // EXPECT_EQ(cmd.as_buffer(), param.buf)
  // << cmd.as_buffer() << " is not equal to " << param.buf;
}

TEST_P(AnalogSendDataTest, from_buffer) {
  auto param = GetParam();

  vn::analog_send_data cmd(param.buf);

  // EXPECT_EQ(cmd.as_buffer(), param.buf)
  // << cmd.as_buffer() << "not equal to " << param.buf;
}

INSTANTIATE_TEST_SUITE_P(
    DasSD, AnalogSendDataTest,
    ::testing::Values(das_sd_p{
        "<DSASD,1,,1506,0000,3279,1235,0194,2527,1215,2396\r\n"_mb,
        mock::f_analog(),
        vn::analog::data(1506, 0, 3279, 1235, 194, 2527, 1215, 2396)}));
