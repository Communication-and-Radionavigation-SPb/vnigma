#include <vnigma/types.h>

#include <gtest/gtest.h>

#define Suite TypeFunctionsTests

namespace vnc = vnigma::core;

TEST(Suite, to_string_analog) {
  vnc::Type type = vnc::Type::analog;

  EXPECT_EQ(vnc::type_to_string(type), "A");
}

TEST(Suite, to_string_serial) {

  vnc::Type type = vnc::Type::serial;

  EXPECT_EQ(vnc::type_to_string(type), "S");
}

TEST(Suite, to_string_digital) {

  vnc::Type type = vnc::Type::digital;

  EXPECT_EQ(vnc::type_to_string(type), "D");
}

TEST(Suite, digital_from_string) {
  std::string str = "D";
  EXPECT_EQ(vnc::type_from_string(str), vnc::digital);
}

TEST(Suite, analog_from_string) {
  std::string str = "A";
  EXPECT_EQ(vnc::type_from_string(str), vnc::analog);
}

TEST(Suite, serial_from_string) {
  std::string str = "S";
  EXPECT_EQ(vnc::type_from_string(str), vnc::serial);
}

TEST(Suite, type_from_string_throws_when_unknown) {
  std::string str = "invalid";
  EXPECT_THROW({ vnc::type_from_string(str); }, std::invalid_argument);
}