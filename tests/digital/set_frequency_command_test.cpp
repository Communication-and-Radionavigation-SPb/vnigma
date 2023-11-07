#include <gtest/gtest.h>

#include <vnigma/vnigma.h>
#include "../mock_generator.hpp"

#include <vnigma/digital/set_frequency_command.h>
#include <vnigma/digital/types.h>

namespace vnc = vnigma::core;
namespace vnd = vnigma::digital;

#define Suite DigitalSetFrequencyTests

struct set_frequency_command_test {
  std::string validation;
  vnd::data::Period data_period;
};

class DigitalSetFrequencyTests
    : public ::testing::TestWithParam<set_frequency_command_test> {
 private:
  vnc::Module* module_;
  vnc::GuidGenerator* generator_;

 public:
  vnd::commands::SetFrequencyCommand create(
      const set_frequency_command_test& param) {
    module_ = new vnc::Module(1, vnc::vn, vnc::digital);
    generator_ = new MockGuidGenerator();

    return vnd::commands::SetFrequencyCommand(module_, generator_,
                                              param.data_period);
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
    {"<VNDSF,1,,-1", vnd::data::never},  {"<VNDSF,1,,500", vnd::data::fastest},
    {"<VNDSF,1,,1000", vnd::data::fast}, {"<VNDSF,1,,2000", vnd::data::normal},
    {"<VNDSF,1,,5000", vnd::data::slow}, {"<VNDSF,1,,10000", vnd::data::slower},
};

INSTANTIATE_TEST_SUITE_P(Container, DigitalSetFrequencyTests,
                         ::testing::ValuesIn(entries));