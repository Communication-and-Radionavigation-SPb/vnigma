#include <gtest/gtest.h>
#include <vnigma/target.h>
#include <sstream>
#include <vector>

#define Suite TargetConversion

namespace vnc = vnigma::core;

struct conversion_entry {
  std::string str;
  vnc::Target target;
  std::string name;
};

class TargetConversion : public ::testing::TestWithParam<conversion_entry> {};

TEST_P(Suite, converts_to_string) {
  conversion_entry entry = GetParam();

  std::string target = vnc::target_to_string(entry.target);

  EXPECT_EQ(target, entry.str);
}

TEST_P(Suite, resolves_from_string) {
  conversion_entry entry = GetParam();

  vnc::Target target = vnc::target_from_string(entry.str);

  EXPECT_EQ(target, entry.target);
}

TEST(Suite, throws_when_unknown_from_string) {
  std::string str = "invalid";
  EXPECT_THROW({ vnc::target_from_string(str); }, std::invalid_argument);
}

const std::vector<conversion_entry> entries = {{"DS", vnc::Target::das, "das"},
                                               {"VN", vnc::Target::vn, "vn"}};

INSTANTIATE_TEST_SUITE_P(
    Conversion, Suite, ::testing::ValuesIn(entries),
    [](const ::testing::TestParamInfo<TargetConversion::ParamType>& info) {
      std::stringstream ss;
      ss << info.param.name << "_" << info.index;
      std::string name = ss.str();
      return name;
    });
