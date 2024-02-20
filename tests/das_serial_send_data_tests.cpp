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

using std::optional;
using std::tuple;
using std::variant;
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

  EXPECT_EQ(cmd.payload(), "data");

  EXPECT_EQ(cmd.port_index(), 3);
  EXPECT_EQ(cmd.get_data().value(), "data");
}

TEST_P(SerialSendDataTests, as_buffer) {
  auto param = GetParam();

  vn::serial_send_data cmd(param.index, param.dev, param.data);
  EXPECT_EQ(cmd.get_device(), param.dev);
  EXPECT_EQ(cmd.as_buffer(), param.buf)
      << cmd.as_buffer() << " is not equal to " << param.buf;
}

TEST_P(SerialSendDataTests, from_buffer) {
  auto param = GetParam();

  vn::serial_send_data cmd(param.buf);

  EXPECT_EQ(cmd.as_buffer(), param.buf)
      << cmd.as_buffer() << "not equal to " << param.buf;
}

INSTANTIATE_TEST_SUITE_P(DasSD, SerialSendDataTests,
                         ::testing::Values(das_sc_sd_p{
                             "<DSSSD,1,3,$GPHDT,274.07,T*03\r\n"_mb,
                             mock::f_serial(),
                             vn::serial::data("$GPHDT,274.07,T*03"), 3}));
