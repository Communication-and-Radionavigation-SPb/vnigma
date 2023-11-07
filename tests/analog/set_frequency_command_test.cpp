#include <gtest/gtest.h>

#include <vnigma/vnigma.h>
#include "../mock_generator.hpp"

#include <vnigma/analog/set_frequency_command.h>
#include <vnigma/analog/types.h>

namespace vnc = vnigma::core;
namespace vna = vnigma::analog;

#define Suite AnalogSetFrequencyTests

struct set_frequency_command_test {
  std::string validation;
  vna::data::Period data_period;
  vna::config::Period config_period;
};

class AnalogSetFrequencyTests
    : public ::testing::TestWithParam<set_frequency_command_test> {
 private:
  vnc::Module* module_;
  vnc::GuidGenerator* generator_;

 public:
  vna::commands::SetFrequencyCommand create(
      const set_frequency_command_test& param) {
    module_ = new vnc::Module(1, vnc::vn, vnc::analog);
    generator_ = new MockGuidGenerator();

    return vna::commands::SetFrequencyCommand(
        module_, generator_, param.data_period, param.config_period);
  }

 protected:
  void TearDown() override {
    if (module_ != nullptr)
      delete module_;
    if (generator_ != nullptr)
      delete generator_;
  }
};

TEST_P(Suite, composed_correctly) {
  auto param = GetParam();

  auto cmd = create(param);

  EXPECT_EQ(cmd.compose(), param.validation);
}

const std::vector<set_frequency_command_test> entries = {
    {"<VNASF,1,,-1,-1", vna::data::never, vna::config::never},
    {"<VNASF,1,,500,-1", vna::data::fastest, vna::config::never},
    {"<VNASF,1,,1000,-1", vna::data::fast, vna::config::never},
    {"<VNASF,1,,2000,-1", vna::data::normal, vna::config::never},
    {"<VNASF,1,,5000,-1", vna::data::slow, vna::config::never},
    {"<VNASF,1,,10000,-1", vna::data::slower, vna::config::never},
    {"<VNASF,1,,-1,2", vna::data::never, vna::config::fastest},
    {"<VNASF,1,,-1,5", vna::data::never, vna::config::fast},
    {"<VNASF,1,,-1,10", vna::data::never, vna::config::slow},
    {"<VNASF,1,,-1,60", vna::data::never, vna::config::slower},
};

INSTANTIATE_TEST_SUITE_P(Container, AnalogSetFrequencyTests,
                         ::testing::ValuesIn(entries));