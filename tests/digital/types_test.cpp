#include <gtest/gtest.h>
#include <algorithm>

#include <vnigma/digital/types.h>

namespace vnd = vnigma::digital;

#define Suite DigitalTypesTests

struct digital_period_p {
  std::string value;
  vnd::data::Period period;
};

class DigitalTypesTests : public ::testing::TestWithParam<digital_period_p> {};

TEST_P(Suite, from_string) {
  auto param = GetParam();
  auto value = vnd::data::from_string(param.value);
  EXPECT_EQ(value, param.period);
}

TEST_P(Suite, to_string) {
  auto param = GetParam();
  auto value = vnd::data::to_string(param.period);
  EXPECT_EQ(value, param.value);
}

const std::vector<digital_period_p> entries = {
    {"-1", vnd::data::never},  {"500", vnd::data::fastest},
    {"1000", vnd::data::fast}, {"2000", vnd::data::normal},
    {"5000", vnd::data::slow}, {"10000", vnd::data::slower}};

INSTANTIATE_TEST_SUITE_P(
    Conversion, Suite, ::testing::ValuesIn(entries),
    [](const ::testing::TestParamInfo<DigitalTypesTests::ParamType>& info) {
      std::stringstream ss;
      ss << info.param.value << "_" << info.index;
      std::string name = ss.str();

      std::replace_if(
          name.begin(), name.end(), [](char c) { return !std::isalnum(c); },
          '_');
      return name;
    });