#include <gtest/gtest.h>
#include <optional>
#include <string>
#include <vnigma/util/number.hpp>

namespace vn = vnigma::util;
using std::optional;

struct dbl_number_p {
  std::string str;
  double expect;
  bool fails;
};

struct int_number_p {
  std::string str;
  int expect;
  bool fails;
};

class toDoubleTest : public ::testing::TestWithParam<dbl_number_p> {};

TEST_P(toDoubleTest, fails) {
  auto param = GetParam();
  if (!param.fails) {
    GTEST_SKIP() << "Skipped cause should not fail";
  }

  EXPECT_THROW({ vn::toDouble(param.str); }, std::exception)
      << "'" << param.str << "' throws nothing";
}

TEST_P(toDoubleTest, convert) {
  auto param = GetParam();
  if (param.fails) {
    GTEST_SKIP() << "Skipped cause should fail";
  }

  auto result = vn::toDouble(param.str);
  EXPECT_EQ(result, param.expect);
}

class toIntegerTest : public ::testing::TestWithParam<int_number_p> {};

TEST_P(toIntegerTest, fails) {
  auto param = GetParam();
  if (!param.fails) {
    GTEST_SKIP() << "Skipped cause should not fail";
  }
  EXPECT_THROW({ vn::toInteger(param.str); }, std::exception)
      << "'" << param.str << "' throws nothing";
}

TEST_P(toIntegerTest, convert) {
  auto param = GetParam();
  if (param.fails) {
    GTEST_SKIP() << "Skipped cause should fail";
  }

  auto result = vn::toInteger(param.str);
  EXPECT_EQ(result, param.expect);
}

INSTANTIATE_TEST_SUITE_P(NumberUtil, toDoubleTest,
                         ::testing::Values(dbl_number_p{"", 0, true},
                                           dbl_number_p{"a", 0, true},
                                           dbl_number_p{"1a", 0, true},
                                           dbl_number_p{"1..0", 0, true},
                                           dbl_number_p{"1.0", 1.0, false},
                                           dbl_number_p{"1", 1.0, false},
                                           dbl_number_p{"1.0 ", 1.0, true}));

INSTANTIATE_TEST_SUITE_P(NumberUtil, toIntegerTest,
                         ::testing::Values(int_number_p{"", 0, true},
                                           int_number_p{"1a", 0, true},
                                           int_number_p{"1.0", 0, true},
                                           int_number_p{" 1", 1, false},
                                           int_number_p{"1 ", 0, true},
                                           int_number_p{"10", 10, false},
                                           int_number_p{"19", 19, false},
                                           int_number_p{"1234", 1234, false},
                                           int_number_p{"0", 0, false},
                                           int_number_p{"00", 0, false}));