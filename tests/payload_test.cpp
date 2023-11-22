#include <gtest/gtest.h>

#include <vnigma/buffer.hpp>
#include <vnigma/data.hpp>
#include <vnigma/exception.hpp>
#include <vnigma/util/parameter_pack.hpp>

namespace vn {
using namespace vnigma;
}  // namespace vn

using namespace vnigma::literals;

class AnalogDataTest : public ::testing::Test {
 public:
  static constexpr int quantity = 8;
};

TEST_F(AnalogDataTest, base_quantity) {
  EXPECT_EQ(vn::analog::data::quantity, 8);
}

TEST_F(AnalogDataTest, from_buffer) {
  vn::buffer buf;

  buf = "1506,0000,3278, 1235 ,0194,2527,1215,2396"_mb;
  EXPECT_THROW({ vn::analog::base_data<7> data(buf); }, vn::system_error);
  EXPECT_THROW({ vn::analog::base_data<9> data(buf); }, vn::system_error);
  ASSERT_NO_THROW({ vn::analog::base_data<8> data(buf); });

  vn::analog::base_data<8> data(buf);
  std::vector<int> expectation{1506, 0, 3278, 1235, 194, 2527, 1215, 2396};

  EXPECT_TRUE(std::equal(data.begin(), data.end(), expectation.begin()));
}

TEST_F(AnalogDataTest, creates_from_variative_args) {
  vn::analog::base_data<2> data(1, 2);
  std::vector<int> expectation{1, 2};

  ASSERT_EQ(data.size(), expectation.size());

  auto it = data.begin();
  auto eit = expectation.begin();
  while (it != data.end()) {
    EXPECT_EQ(*it, *eit);
    ++it;
    ++eit;
  }
}

TEST_F(AnalogDataTest, creates_from_initializer_list) {
  vn::analog::base_data<2> data{1, 2};

  std::vector<int> expectation{1, 2};

  ASSERT_EQ(data.size(), expectation.size());

  EXPECT_TRUE(std::equal(data.begin(), data.end(), expectation.begin()));
}

TEST_F(AnalogDataTest, creates_from_vector) {
  std::vector<int> expectation{1, 2};
  vn::analog::base_data<2> data(expectation);

  EXPECT_TRUE(std::equal(data.begin(), data.end(), expectation.begin()));
}