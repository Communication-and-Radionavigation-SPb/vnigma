#include <gtest/gtest.h>

#include <vnigma/command.h>

#include "mock_generator.hpp"

#include <vnigma/base/reset_command.h>

#define Suite ResetCommandTests

namespace vnc = vnigma::core;
namespace vncm = vnigma::commands;

struct reset_p {
  std::string validation;
  uint8_t id;
  vnc::Target target;
  vnc::Type type;
};

class ResetCommandTests : public ::testing::TestWithParam<reset_p> {
 public:
  vnc::Module* module_;
  vnc::GuidGenerator* generator_;
  vncm::base::ResetCommand* cmd;

 protected:
  void SetUp() {
    auto param = GetParam();

    module_ = new vnigma::core::Module(param.id, param.target, param.type);
    generator_ = new MockGuidGenerator();

    cmd = new vncm::base::ResetCommand(module_, generator_);
  }

  void TearDown() {
    delete cmd;
    delete module_;
    delete generator_;
  }
};

TEST_P(Suite, composed_correctly) {
  auto param = GetParam();

  auto res = cmd->compose();

  std::cout << " [CMD]: " << res << std::endl;

  EXPECT_EQ(res, param.validation);
}

std::vector<reset_p> entries = {
    {"<DSSSR,00000001,1", 1, vnc::Target::das, vnc::Type::serial},
    {"<VNSSR,1", 1, vnc::Target::vn, vnc::Type::serial},
    {"<DSASR,00000001,1", 1, vnc::Target::das, vnc::Type::analog},
    {"<VNASR,1", 1, vnc::Target::vn, vnc::Type::analog},
    {"<DSDSR,00000001,1", 1, vnc::Target::das, vnc::Type::digital},
    {"<VNDSR,1", 1, vnc::Target::vn, vnc::Type::digital},
};

INSTANTIATE_TEST_SUITE_P(ResetCommandCompose, ResetCommandTests,
                         ::testing::ValuesIn(entries));
