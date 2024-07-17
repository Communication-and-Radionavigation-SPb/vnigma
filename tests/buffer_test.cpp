#include <gtest/gtest.h>
#include <iterator>
#include <vnigma/buffer.hpp>

namespace vn = vnigma;

TEST(BufferTests, allocate_const_char_ptr) {
  auto buf = vn::allocate_buffer("01234");
  EXPECT_TRUE(buf == "01234");
  EXPECT_TRUE(buf.get_anchor().has_value());

  auto substr1 = buf.substr(2, 3);
  EXPECT_TRUE(substr1 == "234")
      << "Single substr do not matches expected value";
  EXPECT_TRUE(substr1.get_anchor().has_value())
      << "::substr do not moves anchor value";

  auto substr2 = substr1.substr(1);
  std::cout << substr2;
  EXPECT_EQ(substr2, "34") << "multiple substr do not matches expected value";
  EXPECT_TRUE(substr2.get_anchor().has_value())
      << "::substr multiple times do not moves anchor value";
}

TEST(BufferTests, nullterm_const_char_ptr) {
  std::string s("1234\0");
  ASSERT_EQ(std::distance(s.begin(), s.end()), 4);
  ASSERT_EQ(s.size(), 4);
  auto buf = vn::allocate_buffer(s);
  EXPECT_TRUE(buf == "1234");
  ASSERT_EQ(buf.size(), 4);
}

TEST(BufferTests, allocate_iterators) {
  std::string s{"01234"};
  auto buf = vn::allocate_buffer(s.begin(), s.end());
  EXPECT_TRUE(buf == "01234");
  EXPECT_TRUE(buf.get_anchor().has_value());

  auto substr1 = buf.substr(2, 3);
  EXPECT_EQ(substr1, "234");
  EXPECT_TRUE(substr1.get_anchor().has_value());

  auto substr2 = substr1.substr(1);
  EXPECT_EQ(substr2, "34");
  EXPECT_TRUE(substr2.get_anchor().has_value());
}

TEST(BufferTests, view) {
  std::string s{"01234"};
  vn::buffer buf{std::string_view{s}};

  EXPECT_TRUE(buf == "01234");
  EXPECT_FALSE(buf.get_anchor().has_value());

  auto substr1 = buf.substr(2, 3);

  EXPECT_EQ(substr1, "234");
  EXPECT_FALSE(substr1.get_anchor().has_value());

  auto substr2 = substr1.substr(1);

  EXPECT_EQ(substr2, "34");
  EXPECT_FALSE(substr2.get_anchor().has_value());
}

TEST(BufferTests, literals) {
  using namespace vn::literals;

  auto buf = "01234"_mb;

  EXPECT_TRUE(buf == "01234");
  EXPECT_FALSE(buf.get_anchor().has_value());

  auto substr1 = buf.substr(2, 3);
  EXPECT_TRUE(substr1 == "234");
  EXPECT_FALSE(substr1.get_anchor().has_value());

  auto substr2 = substr1.substr(1);

  EXPECT_TRUE(substr2 == "34");
  EXPECT_FALSE(substr2.get_anchor().has_value());
}
