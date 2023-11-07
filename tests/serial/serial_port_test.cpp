#include <gtest/gtest.h>
#include <vnigma/serial/serial_port.h>
#include <vnigma/serial/types.h>

#define Suite SerialPortTests

namespace vns = vnigma::serial;

struct serial_port_p {
  bool enabled;
  vns::Baud baudrate;
  vns::Iface iface;
  uint8_t hex;
  uint8_t index;
};

class SerialPortTests : public ::testing::TestWithParam<serial_port_p> {
 protected:
  void SetUp() { auto param = GetParam(); }

  vns::Port createFromParam() {
    auto param = GetParam();
    return vns::Port(param.enabled, param.baudrate, param.iface, param.index);
  }
};

TEST_P(Suite, default_index_is_0) {
  auto param = GetParam();
  vns::Port port(param.enabled, param.baudrate, param.iface);
  EXPECT_EQ(port.index, 0);
}

TEST_P(Suite, correctly_stores_params) {
  auto param = GetParam();
  auto port = createFromParam();

  EXPECT_EQ(port.baudrate, param.baudrate);
  EXPECT_EQ(port.iface, param.iface);
  EXPECT_EQ(port.is_enabled, param.enabled);

  EXPECT_EQ(port.index, param.index);
}

TEST_P(Suite, returns_correct_hex) {
  auto param = GetParam();
  auto port = createFromParam();
  EXPECT_EQ(port.as_hex(), param.hex);
}

const std::vector<serial_port_p> entries = {
    {true, vns::Baud2400, vns::Iface::RS422, 0xF3, 1},
    {false, vns::Baud4800, vns::Iface::RS232, 0xE4, 2},
    {true, vns::Baud115200, vns::Iface::RS422, 0xFD, 3}};

INSTANTIATE_TEST_SUITE_P(SerialPort, Suite, ::testing::ValuesIn(entries));