#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vnigma/buffer.hpp>
#include <vnigma/message/das_digital_send_data.h>
#include <vnigma/message/message_traits.hpp>

class DigitalSendDataTest : public ::testing::Test {};

namespace vn = vnigma;

using namespace vnigma::literals;

TEST_F(DigitalSendDataTest, traits) {
  using T = vn::das::digital_send_data;

  EXPECT_FALSE(vn::is_command<T>());
  EXPECT_TRUE(vn::is_response<T>());

  EXPECT_TRUE(vn::das_related<T>());
  EXPECT_FALSE(vn::venom_related<T>());

  EXPECT_TRUE(vn::is_port_missed<T>());
  EXPECT_FALSE(vn::is_port_scoped<T>());

  EXPECT_TRUE(vn::has_payload<T>());

  EXPECT_TRUE(vn::is_data<T>());
}

TEST_F(DigitalSendDataTest, target_device) {
  EXPECT_ANY_THROW({
    vn::das::digital_send_data("<DSASD,1,,FFFF\r\n"_mb);
  });
  EXPECT_ANY_THROW({
    vn::das::digital_send_data("<DSSSD,1,,FFFF\r\n"_mb);
  });
  EXPECT_NO_THROW({
    vn::das::digital_send_data("<DSDSD,1,,FFFF\r\n"_mb);
  });
}

TEST_F(DigitalSendDataTest, interface) {
  vn::das::digital_send_data cmd("<DSDSD,1,,FFFF\r\n"_mb);
  // Device
  EXPECT_EQ(cmd.get_device().id(), 1);
  EXPECT_EQ(cmd.get_device().type(), vn::core::digital);
  // Payload
  EXPECT_EQ(cmd.payload(), "FFFF");
  // Data
  std::vector<bool> actual = cmd.get_data().items();

  EXPECT_THAT(actual, ::testing::SizeIs(16));
  EXPECT_THAT(actual, ::testing::Each(::testing::Eq(true)));
}