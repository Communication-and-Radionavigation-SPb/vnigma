#include <gtest/gtest.h>
#include <vnigma/message/das_scoped_send_data.h>
#include <vnigma/buffer.hpp>
#include <vnigma/message/message_traits.hpp>

#include "mock_devices.hpp"

#include "das_sd_p.h"

namespace vn {
using namespace vnigma;
using vnigma::das::scoped_send_data;
}  // namespace vn

using std::optional;
using std::tuple;
using std::variant;
using namespace vnigma::literals;

struct das_sc_sd_p : public das_sd_p {
  uint8_t index;
};

class ScopedSendDataTest : public ::testing::TestWithParam<das_sc_sd_p> {};

TEST_F(ScopedSendDataTest, traits) {
  EXPECT_FALSE(vn::is_command<vn::scoped_send_data>());
  EXPECT_TRUE(vn::is_response<vn::scoped_send_data>());

  EXPECT_TRUE(vn::das_related<vn::scoped_send_data>());
  EXPECT_FALSE(vn::venom_related<vn::scoped_send_data>());

  EXPECT_FALSE(vn::is_port_missed<vn::scoped_send_data>());
  EXPECT_TRUE(vn::is_port_scoped<vn::scoped_send_data>());

  EXPECT_TRUE(vn::has_payload<vn::scoped_send_data>());
}

TEST_P(ScopedSendDataTest, as_buffer) {
  auto param = GetParam();

  vn::scoped_send_data cmd(param.index, param.dev, param.data);

  EXPECT_EQ(cmd.as_buffer(), param.buf)
      << cmd.as_buffer() << " is not equal to " << param.buf;
}

TEST_P(ScopedSendDataTest, from_buffer) {
  auto param = GetParam();

  vn::scoped_send_data cmd(param.buf);

  EXPECT_EQ(cmd.as_buffer(), param.buf)
      << cmd.as_buffer() << "not equal to " << param.buf;
}

INSTANTIATE_TEST_SUITE_P(DasSD, ScopedSendDataTest,
                         ::testing::Values(das_sc_sd_p{
                             "<DSSSD,1,3,$GPHDT,274.07,T*03\r\n"_mb,
                             mock::f_serial(),
                             vn::serial::data("$GPHDT,274.07,T*03"), 3}));
