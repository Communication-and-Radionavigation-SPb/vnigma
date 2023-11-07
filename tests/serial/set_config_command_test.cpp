#include <gtest/gtest.h>

#include <vnigma/serial/serial_port.h>
#include <vnigma/serial/set_config_command.h>
#include "../mock_generator.hpp"

#define Suite SerialSetConfigCommandTests

namespace vnc = vnigma::core;
namespace vns {
using namespace vnigma::serial;
}  // namespace vns

struct SC_p {
  std::string validation;
  vnc::Target target;
  std::vector<vns::Port> ports;
};

class SerialSetConfigCommandTests : public ::testing::TestWithParam<SC_p> {
 public:
  vnc::Module* module_;
  vnc::GuidGenerator* generator_;

 public:
  vns::commands::SetConfigurationCommand create() {
    auto param = GetParam();
    module_ = new vnc::Module(1, param.target, vnc::Type::serial);
    generator_ = new MockGuidGenerator();
    return vns::commands::SetConfigurationCommand(module_, generator_,
                                                  param.ports);
  }

 protected:
  void TearDown() {
    if (module_ != nullptr)
      delete module_;
    if (generator_ != nullptr)
      delete generator_;
  }
};

TEST_P(Suite, composed_correctly) {
  auto cmd = create();
  auto param = GetParam();

  EXPECT_EQ(cmd.compose(), param.validation);
}

const std::vector<SC_p> entries = {
    {"<DSSSC,00000001,1,F3,,,,,,,",
     vnc::Target::das,
     {
         vns::Port(vns::Enabled, vns::Baud2400, vns::RS422, 0),
     }},
    {"<DSSSC,00000001,1,,E3,,,F3,,E3,",
     vnc::Target::das,
     {
         vns::Port(vns::Enabled, vns::Baud2400, vns::RS232, 6),
         vns::Port(vns::Enabled, vns::Baud2400, vns::RS422, 4),
         vns::Port(vns::Enabled, vns::Baud2400, vns::RS232, 1),
     }},
    {"<VNSSC,1,,,,,,,,F3",
     vnc::Target::vn,
     {
         vns::Port(vns::Enabled, vns::Baud2400, vns::RS422, 7),
     }}};

INSTANTIATE_TEST_SUITE_P(SerialSetConfigCompose, SerialSetConfigCommandTests,
                         ::testing::ValuesIn(entries));