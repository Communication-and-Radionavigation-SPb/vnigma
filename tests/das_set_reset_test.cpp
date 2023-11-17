#include <gtest/gtest.h>
#include <vnigma/message/message_traits.hpp>
#include <vnigma/message/message_variant.hpp>

#define Suite SetResetTests

namespace vn = vnigma;

TEST(Suite, type_is_correct) {
  vn::device dev(1, vn::core::analog);

  EXPECT_TRUE(vnigma::is_command<vnigma::set_reset>());
  EXPECT_FALSE(vnigma::is_response<vnigma::set_reset>());
  EXPECT_FALSE(vnigma::has_payload<vnigma::set_reset>());
  EXPECT_FALSE(vnigma::is_port_missed<vnigma::set_reset>());
  EXPECT_FALSE(vnigma::is_port_scoped<vnigma::set_reset>());

  vnigma::set_reset msg(dev);

  std::string type = vnigma::control_str<msg.type>::value;
  EXPECT_EQ(type, "SR");
}