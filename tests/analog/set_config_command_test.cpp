#include <gtest/gtest.h>
#include <vnigma/analog/set_config_command.h>
#include <vnigma/target.h>
#include <vnigma/types.h>
#include <vnigma/vnigma.h>

#include "../mock_generator.hpp"

#define Suite AnalogConfigurationTests

namespace vnc = vnigma::core;
namespace vna = vnigma::analog;

struct ASC_p {
  std::string validation;
  vnc::Target target;
  std::vector<vna::Port> ports;
};

class AnalogConfigurationTests : public ::testing::TestWithParam<ASC_p> {
 public:
  vnc::Module* module_;
  vnc::GuidGenerator* generator_;

 public:
  vna::commands::SetConfigurationCommand create(const ASC_p& param) {
    module_ = new vnc::Module(1, param.target, vnc::analog);
    generator_ = new MockGuidGenerator();
    return vna::commands::SetConfigurationCommand(module_, generator_,
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

TEST_P(Suite, compose_is_correct) {
  auto param = GetParam();
  auto cmd = create(param);

  EXPECT_EQ(cmd.compose(), param.validation);
}

const std::vector<ASC_p> entries = {{
                                        "<VNASC,1,,FFFF",
                                        vnc::vn,
                                        {
                                            vna::Port(vna::V10_10R47K, 0),
                                            vna::Port(vna::V10_10R47K, 1),
                                            vna::Port(vna::V10_10R47K, 2),
                                            vna::Port(vna::V10_10R47K, 3),
                                            vna::Port(vna::V10_10R47K, 4),
                                            vna::Port(vna::V10_10R47K, 5),
                                            vna::Port(vna::V10_10R47K, 6),
                                            vna::Port(vna::V10_10R47K, 7),
                                        },
                                    },
                                    {
                                        "<VNASC,1,,727C",
                                        vnc::vn,
                                        {
                                            vna::Port(vna::mA0_20R250, 0),
                                            vna::Port(vna::V10_10R47K, 1),
                                            vna::Port(vna::V10_10R47K, 2),
                                            vna::Port(vna::mA0_20R500, 3),
                                            vna::Port(vna::mA0_20R1K, 4),
                                            vna::Port(vna::mA0_20R250, 5),
                                            vna::Port(vna::V10_10R47K, 6),
                                            vna::Port(vna::mA0_20R500, 7),
                                        },
                                    },
                                    {
                                        "<VNASC,1,,0",
                                        vnc::vn,
                                        {
                                            vna::Port(vna::mA0_20R250, 0),
                                            vna::Port(vna::mA0_20R250, 1),
                                            vna::Port(vna::mA0_20R250, 2),
                                            vna::Port(vna::mA0_20R250, 3),
                                            vna::Port(vna::mA0_20R250, 4),
                                            vna::Port(vna::mA0_20R250, 5),
                                            vna::Port(vna::mA0_20R250, 6),
                                            vna::Port(vna::mA0_20R250, 7),
                                        },
                                    }};

INSTANTIATE_TEST_SUITE_P(AnalogModuleConfig, AnalogConfigurationTests,
                         ::testing::ValuesIn(entries));