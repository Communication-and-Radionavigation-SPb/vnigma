#include <gtest/gtest.h>
#include <vnigma/guid_generator.h>

TEST(Suite, can_be_instantiated) {
  const auto generator = vnigma::core::GuidGenerator();
}

TEST(Suite, generates_ids) {
  const auto generator = vnigma::core::GuidGenerator();

  EXPECT_EQ(generator.next(), 1);
  EXPECT_EQ(generator.next(), 2);
}