#include <gtest/gtest.h>
#include <vnigma/message/das_send_data.h>
#include <vnigma/data.hpp>
#include "vnigma/buffer.hpp"

class QuantifiedDataTests : public ::testing::Test {};

TEST_F(QuantifiedDataTests, one) {
  auto buf = vnigma::allocate_buffer("1");
  vnigma::base::quntified_data<int, 1> data(buf);

  auto actual = data.items();
  std::vector<int> expected{1};
  EXPECT_EQ(actual, expected);
}

TEST_F(QuantifiedDataTests, two) {
  auto buf = vnigma::allocate_buffer("1,2");
  vnigma::base::quntified_data<int, 2> data(buf);

  auto actual = data.items();
  std::vector<int> expected{1, 2};
  EXPECT_EQ(actual, expected);
}

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
  std::vector<T> items{1, 2, 3, 4, 5, 6, 7, 8};
  vnigma::analog::data data(items);

  std::vector<T> actual = data.items();

  EXPECT_EQ(actual, items);
}
TEST_F(AnalogDataTests, from_buffer) {
  auto buf = vnigma::allocate_buffer("1,2,3,4,5,6,7,8");
  vnigma::analog::data data(buf);

  using T = vnigma::analog::data::item_type;
  std::vector<T> expected{1, 2, 3, 4, 5, 6, 7, 8};
  auto actual = data.items();
  EXPECT_EQ(actual, expected);
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
  for (size_t i = 0; i < vnigma::digital::data::quantity; i++) {
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

TEST_F(DigitalDataTests, from_buffer) {
  auto buf = vnigma::allocate_buffer("FFFF");
  vnigma::digital::data data(buf);

  using T = vnigma::digital::data::item_type;
  std::vector<T> expected{true, true, true, true, true, true, true, true,
                          true, true, true, true, true, true, true, true};
  auto actual = data.items();
  EXPECT_EQ(actual, expected);
}
