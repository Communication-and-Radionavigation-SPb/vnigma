#include <vnigma/vnigma.h>

#include <gtest/gtest.h>

TEST(add_test, add_1_1)
{
    EXPECT_EQ(vnigma::add(1, 1), 2);
}
