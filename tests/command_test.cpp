#include <gtest/gtest.h>

#include <vnigma/command.h>

#include "mock_generator.hpp"
#include "mock_t_command.h"

#define Suite CommandTests

namespace vnc = vnigma::core;

struct compose_p {
  std::string validation;
  uint8_t id;
  vnigma::core::Target target;
  vnigma::core::Type type;
  std::string payload;
};

class CommandCompositionTest : public ::testing::TestWithParam<compose_p> {
 public:
  vnigma::core::Module* module_;
  MockTCommand* cmd;
  vnigma::core::GuidGenerator* generator_;

 protected:
  void SetUp() {
    auto param = GetParam();
    module_ = new vnigma::core::Module(param.id, param.target, param.type);

    generator_ = new MockGuidGenerator();

    cmd = new MockTCommand(module_, generator_, param.payload);
  }

  void TearDown() {
    delete cmd;
    delete module_;
    delete generator_;
  }
};

TEST_P(CommandCompositionTest, composed_with_payload) {
  auto param = GetParam();

  auto res = cmd->compose();
  EXPECT_EQ(res, param.validation);
}

const std::vector<compose_p> entries = {
    {"<DSSMT,00000001,1,payload", 1, vnc::das, vnc::serial, "payload"},
    {"<VNAMT,2", 2, vnc::vn, vnc::analog, ""},
    {"<DSDMT,00000001,3,120", 3, vnc::das, vnc::digital, "120"},
};

INSTANTIATE_TEST_SUITE_P(Composition, CommandCompositionTest,
                         ::testing::ValuesIn(entries));