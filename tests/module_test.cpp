#include <vnigma/vnigma.h>

#include <gtest/gtest.h>
#include <string>

#define Suite ModuleTests

TEST(Suite, can_be_instantiated) {
  vnigma::core::Module analog(1, vnigma::core::vn, vnigma::core::analog);
}

struct module_pl {
  std::string verification;
  vnigma::core::Target target;
  vnigma::core::Type type;
  uint8_t id;
};

class VnigmaModuleTest : public ::testing::TestWithParam<module_pl> {
 public:
  vnigma::core::Module* m_module;

 protected:
  void SetUp() override {
    auto param = GetParam();
    m_module = new vnigma::core::Module(param.id, param.target, param.type);
  }

  void TearDown() override { delete m_module; }
};

TEST_P(VnigmaModuleTest, prefix_is_correct) {
  auto pfx = m_module->prefix();
  auto param = GetParam();
  EXPECT_EQ(pfx, param.verification);
}

TEST_P(VnigmaModuleTest, id_is_correct) {
  auto param = GetParam();
  EXPECT_EQ(m_module->id(), param.id);
}

TEST_P(VnigmaModuleTest, target_is_correct) {
  auto param = GetParam();
  EXPECT_EQ(m_module->target(), param.target);
}

const std::vector<module_pl> entries = {
    {"DSA", vnigma::core::Target::das, vnigma::core::Type::analog, 1},
    {"VNA", vnigma::core::Target::vn, vnigma::core::Type::analog, 1},
    {"DSA", vnigma::core::Target::das, vnigma::core::Type::analog, 2},
    {"VNA", vnigma::core::Target::vn, vnigma::core::Type::analog, 2},
    {"DSS", vnigma::core::Target::das, vnigma::core::Type::serial, 1},
    {"VNS", vnigma::core::Target::vn, vnigma::core::Type::serial, 1},
    {"DSS", vnigma::core::Target::das, vnigma::core::Type::serial, 2},
    {"VNS", vnigma::core::Target::vn, vnigma::core::Type::serial, 2},
    {"DSD", vnigma::core::Target::das, vnigma::core::Type::digital, 1},
    {"VND", vnigma::core::Target::vn, vnigma::core::Type::digital, 1},
    {"DSD", vnigma::core::Target::das, vnigma::core::Type::digital, 2},
    {"VND", vnigma::core::Target::vn, vnigma::core::Type::digital, 2},
};

INSTANTIATE_TEST_SUITE_P(ModuleP, VnigmaModuleTest,
                         ::testing::ValuesIn(entries));