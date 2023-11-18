#include <gtest/gtest.h>
/* ----------------------------------- STD ---------------------------------- */
#include <optional>
#include <string>
/* --------------------------------- Vnigma --------------------------------- */
#include <vnigma/message/das_get_config.h>
#include <vnigma/buffer.hpp>
#include <vnigma/message/message_traits.hpp>
#include <vnigma/message/message_variant.hpp>

#define Suite GetConfigTest

namespace vn {
using namespace vnigma;
using namespace vnigma::das;
};  // namespace vn
using std::optional;
using std::string;
using namespace vn::literals;

struct das_gc_p {
  vn::uuid uid;
  vn::buffer buf;
  vn::device dev;
  optional<errc> code;
  optional<std::string> error = std::nullopt;
};

class GetConfigTest : public ::testing::TestWithParam<das_gc_p> {};

TEST_F(Suite, traits) {
  EXPECT_TRUE(vn::is_command<vn::get_config>());
  EXPECT_FALSE(vn::is_response<vn::get_config>());

  EXPECT_TRUE(vn::das_related<vn::get_config>());
  EXPECT_FALSE(vn::venom_related<vn::get_config>());

  EXPECT_FALSE(vn::has_payload<vn::get_config>());

  EXPECT_FALSE(vn::is_port_missed<vn::get_config>());
  EXPECT_FALSE(vn::is_port_scoped<vn::get_config>());
}

TEST_P(Suite, as_buffer) {
  auto param = GetParam();
  if (param.code.has_value()) {
    GTEST_SKIP() << "Skip cause should fail";
  }
  vn::get_config msg(param.uid, param.dev);

  auto buf = msg.as_buffer();
  EXPECT_EQ(buf, param.buf) << buf << " is not equal to " << param.buf;
}

TEST_P(Suite, from_buffer) {
  auto param = GetParam();
  if (param.code.has_value()) {
    GTEST_SKIP() << "Skip cause should fail";
  }
  std::cout << "[debug]: " << param.buf << std::endl;
  vn::get_config msg(param.buf);

  vn::buffer buf = msg.as_buffer();
  EXPECT_EQ(buf, param.buf) << buf << " is not equal to " << param.buf;
}

TEST_P(Suite, fail_from_buffer) {
  auto param = GetParam();
  if (!param.code.has_value()) {
    GTEST_SKIP() << "Skip cause should not fail";
  }
  if (!param.error.has_value()) {
    FAIL() << "Invalid error check income data. Error string is apsent";
  }
  auto thr = [&]() {
    try {
      vn::get_config msg(param.buf);
    } catch (vn::system_error& e) {
      std::string error = e.what();
      EXPECT_EQ(e.code(), param.code.value());
      EXPECT_EQ(error, param.error.value());  // end
      throw;
    }
  };
  EXPECT_THROW({ thr(); }, vn::system_error) << param.buf << " throws nothing";
}

INSTANTIATE_TEST_SUITE_P(  // instantiate test suite
    GetConfig, GetConfigTest,
    ::testing::Values(  // values
        das_gc_p{100, "<DSSGC,100,9\r\n"_mb, vn::device(9, vn::core::serial),
                 std::nullopt},
        das_gc_p{
            100, "<DSSGC,100,\r\n"_mb, vn::device(2, vn::core::serial),
            //          ^
            //          device identifier missed
            errc::bad_message,
            "vnigma::control_message GC device identifier apsent: Bad message"},
        das_gc_p{100, "<DSSGC\r\n"_mb, vn::device(2, vn::core::serial),
                 //          ^
                 //          device identifier missed
                 errc::bad_message,
                 "vnigma::control_message GC message structure is malformed: "
                 "Bad message"},
        das_gc_p{
            100, "<DSSGC,\r\n"_mb, vn::device(2, vn::core::serial),
            //          ^
            //          device identifier missed
            errc::bad_message,
            "vnigma::control_message GC device identifier apsent: Bad message"},
        das_gc_p{
            100, "<DSSGC,100\r\n"_mb, vn::device(2, vn::core::serial),
            //              ^
            //              device identifier missed
            errc::bad_message,
            "vnigma::control_message GC device identifier apsent: Bad message"},
        das_gc_p{100, "<DS\r\n"_mb, vn::device(2, vn::core::serial),
                 //       ^
                 //    device type missed
                 errc::protocol_error,
                 "vnigma::control_message GC no device clarifier: Protocol error"},
        das_gc_p{100, "<DS*GC,100,2\r\n"_mb, vn::device(2, vn::core::serial),
                 //       ^
                 //       wrong device type
                 errc::bad_message,
                 "vnigma::control_message GC could not extract any known "
                 "module type from `*` value: Bad message"},
        das_gc_p{100, ""_mb, vn::device(2, vn::core::serial),
                 errc::protocol_error,
                 "vnigma::control_message GC header is apsent: "
                 "Protocol error"},
        das_gc_p{100, "*DSSGC,100,2\r\n"_mb, vn::device(3, vn::core::serial),
                 //    ^
                 //    wrong prefix
                 errc::bad_message,
                 "vnigma::control_message GC wrong prefix: Bad "
                 "message"

        }  // end
        )  // values
);