#include <vnigma/vnigma.h>

#include <gtest/gtest.h>

#define Suite ModuleTests

TEST(Suite, can_be_instantiated) {
  vnigma::core::Module analog(1, vnigma::core::vn, vnigma::core::analog);
}

TEST(Suite, creates_analog_das) {
  vnigma::core::Module analog(1, vnigma::core::das, vnigma::core::analog);

  EXPECT_EQ(analog.id, 1);
  EXPECT_EQ(analog.type, vnigma::core::Type::analog);
  EXPECT_EQ(analog.target, vnigma::core::Target::das);
}

TEST(Suite, creates_analog_venom) {
  vnigma::core::Module analog(1, vnigma::core::das, vnigma::core::analog);

  EXPECT_EQ(analog.id, 1);
  EXPECT_EQ(analog.type, vnigma::core::Type::analog);
  EXPECT_EQ(analog.target, vnigma::core::Target::das);
}

TEST(Suite, creates_serial_das) {
  vnigma::core::Module serial(1, vnigma::core::das, vnigma::core::serial);

  EXPECT_EQ(serial.id, 1);
  EXPECT_EQ(serial.type, vnigma::core::Type::serial);
  EXPECT_EQ(serial.target, vnigma::core::Target::das);
}

TEST(Suite, creates_serial_venom) {
  vnigma::core::Module serial(1, vnigma::core::vn, vnigma::core::serial);

  EXPECT_EQ(serial.id, 1);
  EXPECT_EQ(serial.type, vnigma::core::Type::serial);
  EXPECT_EQ(serial.target, vnigma::core::Target::vn);
}

TEST(Suite, creates_digital_das) {
  vnigma::core::Module digital(3, vnigma::core::das, vnigma::core::digital);

  EXPECT_EQ(digital.id, 3);
  EXPECT_EQ(digital.type, vnigma::core::Type::digital);
  EXPECT_EQ(digital.target, vnigma::core::Target::das);
}

TEST(Suite, creates_digital_venom) {
  vnigma::core::Module digital(3, vnigma::core::vn, vnigma::core::digital);

  EXPECT_EQ(digital.id, 3);
  EXPECT_EQ(digital.type, vnigma::core::Type::digital);
  EXPECT_EQ(digital.target, vnigma::core::Target::vn);
}