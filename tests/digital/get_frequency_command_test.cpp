#include <gtest/gtest.h>

#include <vnigma/digital/get_frequency_command.h>
#include "../mock_generator.hpp"

#define Suite DigitalGetFrequencyCommandTests

TEST(Suite, composed_correctly) {
  vnigma::core::Module* module_ = new vnigma::core::Module(
      1, vnigma::core::Target::vn, vnigma::core::Type::digital);

  MockGuidGenerator* generator_ = new MockGuidGenerator();

  vnigma::digital::commands::GetFrequencyCommand cmd(module_, generator_);

  EXPECT_EQ(cmd.compose(), "<VNDGF,1");

  delete module_;
  delete generator_;
}