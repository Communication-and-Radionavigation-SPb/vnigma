#include <gtest/gtest.h>
#include <vnigma/ports/serial_port.h>
#include <vnigma/util/binary.hpp>

#define Suite SerialPortTests

using namespace vnigma::serial;

template <unsigned long N>
using bin = vnigma::binary<N>;

TEST(Suite, constructor) {
  port p1(1);

  const auto default_config = bin<11110111>::value;
  EXPECT_EQ(p1.index, 1);
  EXPECT_EQ(p1.configuration, default_config);

  port p2(2, bin<11111100>::value);
  EXPECT_EQ(p2.index, 2);
  EXPECT_EQ(p2.configuration, 0xFC);
}

TEST(Suite, enabled) {
  port enabled(1, bin<11111101>::value);
  EXPECT_TRUE(enabled.is_enabled());

  port disabled(1, bin<11111100>::value);
  EXPECT_FALSE(disabled.is_enabled());
}

TEST(Suite, enable_disable) {
  port p(1);
  auto enabled_mask = 0x1;

  // Port is enabled by default
  ASSERT_EQ(p.configuration & enabled_mask, 1);

  p.disable();
  EXPECT_EQ(p.configuration & enabled_mask, 0);

  p.enable();  // last bit should become 1
  EXPECT_EQ(p.configuration & enabled_mask, 1);
}

TEST(Suite, baud_rate) {
  port port2400(1, bin<11110011>::value);
  port port4800(1, bin<11110101>::value);
  port port9600(1, bin<11110111>::value);
  port port19200(1, bin<11111001>::value);
  port port38400(1, bin<11111011>::value);
  port port115200(1, bin<11111101>::value);

  EXPECT_EQ(port2400.baud_rate(), Baud2400);
  EXPECT_EQ(port4800.baud_rate(), Baud4800);
  EXPECT_EQ(port9600.baud_rate(), Baud9600);
  EXPECT_EQ(port19200.baud_rate(), Baud19200);
  EXPECT_EQ(port38400.baud_rate(), Baud38400);
  EXPECT_EQ(port115200.baud_rate(), Baud115200);
}
TEST(Suite, set_baud_rate) {
  port p(1);
  unsigned expectation = 0;
  expectation = bin<11110011>::value;
  p.set_baud_rate(Baud2400);
  EXPECT_EQ(p.configuration, expectation);

  expectation = bin<11110101>::value;
  p.set_baud_rate(Baud4800);
  EXPECT_EQ(p.configuration, expectation);

  expectation = bin<11110111>::value;
  p.set_baud_rate(Baud9600);
  EXPECT_EQ(p.configuration, expectation);

  expectation = bin<11111001>::value;
  p.set_baud_rate(Baud19200);
  EXPECT_EQ(p.configuration, expectation);

  expectation = bin<11111011>::value;
  p.set_baud_rate(Baud38400);
  EXPECT_EQ(p.configuration, expectation);

  expectation = bin<11111101>::value;
  p.set_baud_rate(Baud115200);
  EXPECT_EQ(p.configuration, expectation);
}