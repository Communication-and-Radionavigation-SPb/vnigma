#include <gtest/gtest.h>

#include <vnigma/util/binary.hpp>

#define Suite BinaryStructValueTest

TEST(Suite, edge_case) {

  unsigned const zero = vnigma::binary<0>::value;

  EXPECT_EQ(zero, 0);
}

TEST(Suite, correctly_calculates_value) {
  unsigned const v1 = vnigma::binary<1>::value;
  unsigned const v2 = vnigma::binary<10>::value;
  unsigned const v243 = vnigma::binary<11110011>::value;

  EXPECT_EQ(v1, 1);
  EXPECT_EQ(v2, 2);
  EXPECT_EQ(v243, 243);
}
