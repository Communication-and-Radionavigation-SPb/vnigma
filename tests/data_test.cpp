#include <gtest/gtest.h>
#include <vnigma/data.hpp>

class AnalogDataTests : public ::testing::Test {};

TEST_F(AnalogDataTests, item_type) {
  constexpr bool type_correct =
      std::is_same_v<vnigma::analog::data::item_type, int>;
  ASSERT_TRUE(type_correct);
}

TEST_F(AnalogDataTests, quantity) {
  ASSERT_EQ(vnigma::analog::data::quantity, 8);
}

TEST_F(AnalogDataTests, at_port) {
  using T = vnigma::analog::data::item_type;
  std::vector<T> items{1, 2, 3, 4, 5, 6, 7, 8};
  vnigma::analog::data data(items);
  for (size_t i = 0; i < vnigma::analog::data::quantity; i++) {
    EXPECT_EQ(data.at(i), i + 1);
  }
}

TEST_F(AnalogDataTests, as_vector) {
  using T = vnigma::analog::data::item_type;
  std::vector<T> items{1,2,3,4,5,6,7,8};
  vnigma::analog::data data(items);

  std::vector<T> actual = data.items();

  EXPECT_EQ(actual, items);
}

class DigitalDataTests : public ::testing::Test {};

TEST_F(DigitalDataTests, item_type) {
  constexpr bool type_correct =
      std::is_same_v<vnigma::digital::data::item_type, bool>;
  ASSERT_TRUE(type_correct);
}

TEST_F(DigitalDataTests, quantity) {
  ASSERT_EQ(vnigma::digital::data::quantity, 16);
}

TEST_F(DigitalDataTests, at_port) {
  using T = vnigma::digital::data::item_type;
  std::vector<T> items{true, false, true, false, true, false, true, false,
                       true, false, true, false, true, false, true, false};

  vnigma::digital::data data(items);
  for (size_t i = 0; i < vnigma::digital::data::quantity; i++)
  {
    bool expected = i % 2 > 0 ? false : true;
    EXPECT_EQ(data.at(i), expected);
  }
}

TEST_F(DigitalDataTests, as_vector) {
  using T = vnigma::digital::data::item_type;
  std::vector<T> items(16, true);

  vnigma::digital::data data(items);

  std::vector<T> actual = data.items();

  EXPECT_EQ(actual, items);
}