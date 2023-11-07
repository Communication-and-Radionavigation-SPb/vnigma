#include <gtest/gtest.h>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

#define Suite DigitalFromString

TEST(Suite, hex_to_bool_by_ports) {
  std::string raw = "FFFFFFFE";

  uint32_t data;
  std::stringstream ss;
  ss << std::hex << raw;
  ss >> data;

  const size_t PORST_COUNT = 24;

  const std::vector<bool> port_states = {
      false, true, true, true, true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true, true, true, true,
  };
  bool mask = true;
  for (size_t i = 0; i < PORST_COUNT; i++) {
    bool enabled = data & mask;
    data = data >> 1;
    EXPECT_EQ(port_states[i], enabled) << "port state invalid at " << i;
  }


}