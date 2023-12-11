#include <gtest/gtest.h>
#include <vnigma/message/das_handshake.h>
#include <vnigma/buffer.hpp>
#include <vnigma/message/message_traits.hpp>

#include "mock_devices.hpp"

namespace vn {
using namespace vnigma;
using vnigma::das::handshake;
}  // namespace vn

using std::optional;
using std::tuple;
using std::variant;
using namespace vn::literals;

struct das_handshake_test {
  vn::buffer buf;
  vn::das::handshake::serial_number_t serialnum;
};

class HandshakeTests : public ::testing::TestWithParam<das_handshake_test> {};

TEST_F(HandshakeTests, traits) {
  EXPECT_FALSE(vn::is_command<vn::handshake>());
  EXPECT_TRUE(vn::is_response<vn::handshake>());
  EXPECT_TRUE(vn::is_service<vn::handshake>());

  EXPECT_TRUE(vn::das_related<vn::handshake>());
  EXPECT_FALSE(vn::venom_related<vn::handshake>());

  EXPECT_FALSE(vn::is_port_missed<vn::handshake>());
  EXPECT_FALSE(vn::is_port_scoped<vn::handshake>());

  EXPECT_TRUE(vn::has_payload<vn::handshake>());
}

TEST_P(HandshakeTests, from_buffer) {
  auto param = GetParam();

  vn::handshake cmd(param.buf);

  auto buf = cmd.as_buffer();
  std::string payload(buf.begin(), buf.end());
  std::string validation(param.buf.begin(), param.buf.end());
  EXPECT_EQ(cmd.serial_number(), param.serialnum);
  EXPECT_EQ(payload, validation);
}

TEST_P(HandshakeTests, as_buffer) {
  auto param = GetParam();
  vn::handshake cmd(param.serialnum);

  EXPECT_EQ(cmd.as_buffer(), param.buf)
      << cmd.as_buffer() << " is not equal to " << param.buf;
}

INSTANTIATE_TEST_SUITE_P(DasHSH, HandshakeTests,
                         ::testing::Values(das_handshake_test{
                             "<DSXSN,ae56ywer\r\n"_mb, "ae56ywer"}));
