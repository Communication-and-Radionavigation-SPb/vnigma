#include <gtest/gtest.h>
#include <vnigma/util/buffer_manip.h>

namespace vn {
using namespace vnigma;
}  // namespace vn

using namespace vn::literals;

struct buffer_manip_test {
  vn::buffer buf;
  vn::buffer exp;
  size_t count;
};

class BufferManipTest : public ::testing::TestWithParam<buffer_manip_test> {};

TEST_F(BufferManipTest, trim_buffer) {
  vn::buffer buf = "content\r\n"_mb;
  vn::buffer exp = "content"_mb;
  EXPECT_EQ(vn::trim_buffer(buf), exp);

  buf = "\r\n"_mb;
  exp = ""_mb;

  EXPECT_EQ(vn::trim_buffer(buf), exp);
}

TEST_P(BufferManipTest, skip) {
  auto param = GetParam();
  auto buf = vn::skip(param.buf, param.count);

  EXPECT_EQ(buf, param.exp);
}

const std::vector<buffer_manip_test> entries = {
    {"<DSSSD,111,2,3,data\r\n"_mb, "3,data\r\n"_mb, 3},
    {"<DSSSD,111,2,3,data\r\n"_mb, "2,3,data\r\n"_mb, 2},
    {"<DSSSD,"_mb, ""_mb, 2},
    {""_mb, ""_mb, 2}};

INSTANTIATE_TEST_SUITE_P(BufferManip, BufferManipTest,
                         ::testing::ValuesIn(entries));