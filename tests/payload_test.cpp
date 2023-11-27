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

  buf = "1506,0000,3278,1235,0194,2527,1215,2396"_mb;
  EXPECT_THROW({ vn::analog::data data("15,12,13"_mb); }, vn::system_error)
      << "No throw when amount of fields less than quantity";
  EXPECT_THROW({ vn::analog::data data("1,2,3,4,5,6,7,8,9"_mb); },
               vn::system_error)
      << "No throw when amount of fields greather than quantity";
  ASSERT_NO_THROW({ vn::analog::data data(buf); });

  vn::analog::data data(buf);
  std::vector<int> expectation{1506, 0, 3278, 1235, 194, 2527, 1215, 2396};

  EXPECT_TRUE(std::equal(data.begin(), data.end(), expectation.begin()));
}

TEST_F(AnalogDataTest, constructor) {
  std::vector<int> expectation{1, 2, 3, 4, 5, 6, 7, 8};
  vn::analog::data data1(1, 2, 3, 4, 5, 6, 7, 8);
  vn::analog::data data2{1, 2, 3, 4, 5, 6, 7, 8};
  vn::analog::data data3(expectation);

  ASSERT_EQ(data1.size(), expectation.size());
  ASSERT_EQ(data2.size(), expectation.size());
  ASSERT_EQ(data3.size(), expectation.size());

  EXPECT_TRUE(std::equal(data1.begin(), data1.end(), expectation.begin()));
  EXPECT_TRUE(std::equal(data2.begin(), data2.end(), expectation.begin()));
  EXPECT_TRUE(std::equal(data3.begin(), data3.end(), expectation.begin()));
}

class DigitalDataTest : public ::testing::Test {
 public:
  static constexpr int quantity = 16;
};

TEST_F(DigitalDataTest, base_quantity) {
  EXPECT_EQ(vn::digital::data::quantity, quantity);
}

TEST_F(DigitalDataTest, from_buffer) {

}

TEST_F(DigitalDataTest, constructor) {
  std::vector<bool> expectation = {
      true,  true,  false, false, true, true, false, true,
      false, false, true,  false, true, true, true,  true,
  };

  vn::digital::data data1(true, true, false, false, true, true, false, true,
                          false, false, true, false, true, true, true, true);
  vn::digital::data data2{true,  true,  false, false, true, true, false, true,
                          false, false, true,  false, true, true, true,  true};
  vn::digital::data data3(expectation);

  ASSERT_EQ(data1.size(), expectation.size());
  ASSERT_EQ(data2.size(), expectation.size());
  ASSERT_EQ(data3.size(), expectation.size());

  EXPECT_TRUE(std::equal(data1.begin(), data1.end(), expectation.begin()));
  EXPECT_TRUE(std::equal(data2.begin(), data2.end(), expectation.begin()));
  EXPECT_TRUE(std::equal(data3.begin(), data3.end(), expectation.begin()));
}