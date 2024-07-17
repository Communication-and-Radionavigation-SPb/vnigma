#include <gtest/gtest.h>
#include <vnigma/message/das_serial_send_data.h>
#include <vnigma/buffer.hpp>
#include <vnigma/message/message_traits.hpp>

#include "mock_devices.hpp"

#include "das_sd_p.h"

namespace vn {
using namespace vnigma;
using vnigma::das::serial_send_data;
}  // namespace vn

using namespace vnigma::literals;

struct das_sc_sd_p : public das_sd_p {
  uint8_t index;
};

class SerialSendDataTests : public ::testing::TestWithParam<das_sc_sd_p> {};

TEST_F(SerialSendDataTests, traits) {
  EXPECT_FALSE(vn::is_command<vn::serial_send_data>());
  EXPECT_TRUE(vn::is_response<vn::serial_send_data>());

  EXPECT_TRUE(vn::das_related<vn::serial_send_data>());
  EXPECT_FALSE(vn::venom_related<vn::serial_send_data>());

  EXPECT_FALSE(vn::is_port_missed<vn::serial_send_data>());
  EXPECT_TRUE(vn::is_port_scoped<vn::serial_send_data>());

  EXPECT_TRUE(vn::has_payload<vn::serial_send_data>());

  EXPECT_TRUE(vn::is_data<vn::serial_send_data>());
}

TEST_F(SerialSendDataTests, target_type) {
  EXPECT_NO_THROW({ vn::serial_send_data("<DSSSD,1,3,data\r\n"_mb); });
  EXPECT_ANY_THROW({ vn::serial_send_data("<DSASD,1,3,data\r\n"_mb); });
  EXPECT_ANY_THROW({ vn::serial_send_data("<DSDSD,1,3,data\r\n"_mb); });
}
TEST_F(SerialSendDataTests, correctly_resolves_buffer) {
  vn::serial_send_data cmd("<DSSSD,1,3,data\r\n"_mb);

  EXPECT_EQ(cmd.get_device().id(), 1);
  EXPECT_EQ(cmd.get_device().type(), vn::Type::serial);

  EXPECT_EQ(cmd.payload(), "data\r\n");

  EXPECT_EQ(cmd.port_index(), 3);
  EXPECT_EQ(cmd.get_data().value(), "data\r\n");
}

TEST_P(SerialSendDataTests, as_buffer) {
  auto param = GetParam();

  std::string expected(param.buf.begin(), param.buf.end());

  vn::serial_send_data cmd(param.index, param.dev, param.data);
  auto buf = cmd.as_buffer();
  std::string actual(buf.begin(), buf.end());

  EXPECT_EQ(cmd.get_device().id(), param.dev.id());
  EXPECT_EQ(cmd.get_device().type(), param.dev.type());
  EXPECT_EQ(actual, expected)
      << cmd.as_buffer() << " is not equal to " << param.buf;
}

TEST_F(SerialSendDataTests, from_buffer_str) {
  auto buf = vnigma::allocate_buffer("<DSSSD,1,3,$GPHDT,274.07,T*03");
  vn::serial_send_data msg(buf);

  ASSERT_EQ(msg.payload(), "$GPHDT,274.07,T*03");
}

TEST_P(SerialSendDataTests, from_buffer) {
  auto param = GetParam();

  vn::serial_send_data cmd(param.buf);
  auto buf = cmd.as_buffer();
  auto actual = std::string(buf.begin(), buf.end());
  auto expected = std::string(param.buf.begin(), param.buf.end());

  EXPECT_EQ(actual, expected) << actual << "not equal to " << expected;
}

INSTANTIATE_TEST_SUITE_P(DasSD, SerialSendDataTests,
                         ::testing::Values(das_sc_sd_p{
                             "<DSSSD,1,3,$GPHDT,274.07,T*03\r\n"_mb,
                             mock::f_serial(),
                             vn::serial::data("$GPHDT,274.07,T*03\r\n"), 3}));
