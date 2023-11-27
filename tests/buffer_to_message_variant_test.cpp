#include <gtest/gtest.h>

#include <vnigma/buffer_to_message_variant.h>
#include <iostream>
#include <vnigma/buffer.hpp>
#include <vnigma/exception.hpp>

namespace vn = vnigma;
using namespace vn::literals;

#define Suite BufferToMesVar

struct err_m {
  vn::buffer buf;
  errc code;
};

class BufferErrTest : public ::testing::TestWithParam<err_m> {};

TEST_P(BufferErrTest, returns_error) {
  auto param = GetParam();
  auto msg = vn::buffer_to_message_variant(param.buf);
  EXPECT_FALSE((bool)msg);
  ASSERT_NE(msg.get_if<vn::system_error>(), nullptr);
  EXPECT_EQ(msg.get<vn::system_error>().code(), param.code);
}

INSTANTIATE_TEST_SUITE_P(  // errors suite
    Errors, BufferErrTest,
    ::testing::Values(err_m{"<"_mb, errc::message_size},       // size
                      err_m{"<VN"_mb, errc::message_size},     // size
                      err_m{"<VNS"_mb, errc::message_size},    // size
                      err_m{"<VNS**"_mb, errc::message_size},  // size
                      // not supported
                      err_m{"<VNS**,1"_mb, errc::not_supported}  // end
                      ));

class BufferConvertTest : public ::testing::TestWithParam<vn::buffer> {};

TEST_P(BufferConvertTest, converts) {
  auto param = GetParam();
  auto cmd = vn::buffer_to_message_variant(param);

  EXPECT_TRUE((bool)cmd) << cmd.get<vn::system_error>().what();

  std::optional<vn::buffer> buf = cmd.visit(vn::overload{
      [](auto& p) -> std::optional<vn::buffer> { return p.as_buffer(); },
      [](vn::system_error& e) -> std::optional<vn::buffer>
      {
        return std::nullopt;
      }});

  EXPECT_TRUE(buf.has_value());
  EXPECT_EQ(buf, param);
}

INSTANTIATE_TEST_SUITE_P(
    Buffer, BufferConvertTest,
    ::testing::Values(
        // set frequency
        "<DSASF,100,1,,500\r\n"_mb,   //
        "<DSDSF,100,1,,1000\r\n"_mb,  //
        // set config
        "<DSSSC,100,2,5,FC\r\n"_mb,  //
        // send data
        "<DSASD,0,,1506,0000,3278,1235,0194,2527,1215,2396\r\n"_mb,  //
        "<DSDSD,5,,44ED\r\n"_mb,                                     //
        "<DSSSD,3,1,$GPHDT,127.09,T*03\r\n"_mb,                      //
        // set reset
        "<DSSSR,100005,1\r\n"_mb,           //
        "<DSASR,100005,1\r\n"_mb,           //
        "<DSDSR,100005,1\r\n"_mb,           //
        "<DSSGC,1003,2\r\n"_mb,             //
        "<DSSAK,1003,2,,WRONG_DATA\r\n"_mb  //
        ));
