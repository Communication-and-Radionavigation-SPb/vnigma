#include <gtest/gtest.h>
#include <sstream>
#include <vnigma/control_type.hpp>

#define Suite ControlStr

namespace vn = vnigma;
namespace vnd = vnigma::das;

TEST(Suite, value) {
  std::string greed_send_data = vn::control_str<vnd::greed_send_data>::value;
  EXPECT_TRUE(greed_send_data == "SD");

  std::string scoped_send_data = vn::control_str<vnd::scoped_send_data>::value;
  EXPECT_TRUE(scoped_send_data == "SD");

  std::string set_frequency = vn::control_str<vnd::set_frequency>::value;
  EXPECT_TRUE(set_frequency == "SF");

  std::string set_reset = vn::control_str<vnd::set_reset>::value;
  EXPECT_TRUE(set_reset == "SR");

  std::string set_config = vn::control_str<vnd::set_config>::value;
  EXPECT_TRUE(set_config == "SC");

  std::string get_config = vn::control_str<vnd::get_config>::value;
  EXPECT_TRUE(get_config == "GC");

  std::string ack = vn::control_str<vnd::ack>::value;
  EXPECT_TRUE(ack == "AK");

  std::string handshake = vn::control_str<vnd::handshake>::value;
  EXPECT_TRUE(handshake == "SN");
}