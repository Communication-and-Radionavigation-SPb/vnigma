#include <gtest/gtest.h>
#include <vnigma/message/das_message.h>
#include <optional>
#include <tuple>

namespace vn = vnigma;
namespace vnd = vn::das;

using std::optional;
using std::tuple;
using namespace vn::literals;

class TBase : public vnd::base {
 public:
  TBase(vn::buffer buf) : vnd::base(buf) {}
};

struct das_message_test {
  vn::buffer buf;
  vn::uuid expected;
  optional<tuple<errc, std::string>> err;
};

class DasBaseTest : public ::testing::TestWithParam<das_message_test> {};

TEST_P(DasBaseTest, from_buf) {
  auto param = GetParam();
  if (param.err.has_value()) {
    GTEST_SKIP() << "Skip cause should fail";
  }

  TBase tb(param.buf);
  EXPECT_EQ(tb.get_uuid(), param.expected);
}

TEST_P(DasBaseTest, from_buf_fails) {
  auto param = GetParam();
  if (!param.err.has_value()) {
    GTEST_SKIP() << "Skip cause should not fail";
  }

  auto thr = [&]() {
    try {
      TBase tb(param.buf);
    } catch (const vn::system_error& e) {
      std::string error = e.what();
      EXPECT_EQ(e.code(), std::get<0>(param.err.value()));
      EXPECT_EQ(error, std::get<1>(param.err.value()));
      throw;
    }
  };

  EXPECT_THROW({ thr(); }, vn::system_error);
}

INSTANTIATE_TEST_SUITE_P(
    das_uuid, DasBaseTest,
    ::testing::Values(
        das_message_test{"<DSSAK,100,1,OK\r\n", 100, std::nullopt},
        das_message_test{
            "<DSSAK,bad,1,OK\r\n"_mb, 0,
            std::make_tuple(errc::bad_message,
                            "vnigma::das::base failed to create uuid from "
                            "`bad`: Bad message")},  // no uuid
        das_message_test{
            "<DSSAK\r\n"_mb, 0,
            std::make_tuple(
                errc::bad_message,
                "vnigma::das::base uuid part missed: Bad message")},  // no uuid
        das_message_test{
            "<DSSAK,\r\n"_mb, 0,
            std::make_tuple(
                errc::bad_message,
                "vnigma::das::base uuid part missed: Bad message")},  // no uuid
        das_message_test{
            "<DSSAK,,1\r\n"_mb, 0,
            std::make_tuple(
                errc::bad_message,
                "vnigma::das::base uuid part missed: Bad message")},  // no uuid
        das_message_test{
            "<DSSAK,,1,OK\r\n"_mb, 0,
            std::make_tuple(
                errc::bad_message,
                "vnigma::das::base uuid part missed: Bad message")},  // no uuid
        das_message_test{
            ""_mb, 0,
            std::make_tuple(errc::bad_message,
                            "vnigma::das::base can not be created from empty "
                            "buffer: Bad message")}  // empty
        ));