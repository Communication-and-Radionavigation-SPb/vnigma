#include <gtest/gtest.h>
#include <vnigma/message/set_config.h>
#include <vnigma/message/message_traits.hpp>
#include <vnigma/message/message_variant.hpp>
#include <vnigma/util/binary.hpp>

#define Suite SetConfigTests

namespace vn = vnigma;

TEST(Suite, constructor) {
  vn::device dev(1, vn::core::serial);
  vn::serial::port p(1);
  vn::set_config m1(dev, 1), m2(dev, vn::force_move(p)),
      m3(dev, 1, vn::binary<1111>::value);
}

TEST(Suite, type) {
  EXPECT_TRUE(vn::is_command<vn::set_config>());
  EXPECT_FALSE(vn::is_response<vn::set_config>());
  EXPECT_TRUE(vn::has_payload<vn::set_config>());
  EXPECT_FALSE(vn::is_port_missed<vn::set_config>());
  EXPECT_TRUE(vn::is_port_scoped<vn::set_config>());

  vn::device dev(1, vn::core::serial);
  vn::set_config msg(dev, 1);
  std::string type = vn::control_str<msg.type>::value;
  EXPECT_EQ(type, "SC");
}

TEST(Suite, payload) {
  vn::serial::port p1(1, vn::binary<11111101>::value),
      p2(1, vn::binary<11110010>::value);

  vn::device dev(1, vn::core::serial);
  vn::set_config msg(dev, vn::force_move(p1));
  EXPECT_EQ(msg.payload(), "FD");

  msg.set_port(p2);
  EXPECT_EQ(msg.payload(), "F2");
}

TEST(Suite, port) {
  vn::device dev(1, vn::core::serial);
  vn::set_config msg(dev, 1);
  EXPECT_EQ(msg.port(), "1");
}

TEST(Suite, message_variant_containable) {
  vn::device dev(1, vn::core::serial);
  vn::set_config msg(dev, 1);
  vn::message_variant variant(msg);

  EXPECT_NE(variant.get_if<vn::set_config>(), nullptr);
}