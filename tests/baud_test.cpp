#include <gtest/gtest.h>
#include <vnigma/baud.h>
#include <vnigma/util/binary.hpp>

#define Suite BaudTests

namespace vns = vnigma::serial;

TEST(Suite, bin_baud) {
  const auto v2400 = vnigma::binary<1>::value;
  const auto v4800 = vnigma::binary<10>::value;
  const auto v9600 = vnigma::binary<11>::value;
  const auto v19200 = vnigma::binary<100>::value;
  const auto v38400 = vnigma::binary<101>::value;
  const auto v115200 = vnigma::binary<110>::value;

  EXPECT_EQ(vns::bin_baud_rate<vns::Baud2400>::value, v2400);
  EXPECT_EQ(vns::bin_baud_rate<vns::Baud4800>::value, v4800);
  EXPECT_EQ(vns::bin_baud_rate<vns::Baud9600>::value, v9600);
  EXPECT_EQ(vns::bin_baud_rate<vns::Baud19200>::value, v19200);
  EXPECT_EQ(vns::bin_baud_rate<vns::Baud38400>::value, v38400);
  EXPECT_EQ(vns::bin_baud_rate<vns::Baud115200>::value, v115200);
}