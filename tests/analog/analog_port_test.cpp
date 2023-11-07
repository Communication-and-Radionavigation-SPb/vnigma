#include <gtest/gtest.h>

#include <vnigma/analog/port.h>
#include <vnigma/analog/types.h>

#define Suite AnalogPortTests

namespace vna = vnigma::analog;

struct analog_port_p {
  vna::Runmode mode;
  uint8_t index;
  uint8_t hex;
};

class AnalogPortTests : public ::testing::TestWithParam<analog_port_p> {
 protected:
  vna::Port createFromParam(const analog_port_p& param) {
    return vna::Port(param.mode, param.index);
  }
};


TEST_P(Suite, default_index_is_0) {
  auto param = GetParam();
  vna::Port port, portParam(param.mode);

  EXPECT_EQ(port.index, 0);
  EXPECT_EQ(portParam.index, 0);
}

TEST_P(Suite, correctly_stores_params) {
  auto param = GetParam();
  auto port = createFromParam(param);

  EXPECT_EQ(port.getMode(), param.mode);
  EXPECT_EQ(port.index, param.index);
}

TEST_P(Suite, returns_correct_hex) {
  auto param = GetParam();

  auto port = createFromParam(param);
  EXPECT_EQ(port.as_hex(), param.hex);
}

const std::vector<analog_port_p> entries = {
    {vna::mA0_20R250, 1, 0},
    {vna::mA0_20R500, 1, 1},
    {vna::Runmode::mA0_20R1K, 1, (uint8_t)0x2},
    {vna::Runmode::V10_10R47K, 2, (uint8_t)0x3}
    // end
};

INSTANTIATE_TEST_SUITE_P(AnalogPort, Suite, ::testing::ValuesIn(entries));
