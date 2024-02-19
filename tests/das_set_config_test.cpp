#include <gtest/gtest.h>
#include <vnigma/message/das_set_config.h>
#include <optional>
#include <tuple>
#include <variant>
#include <vnigma/message/message_traits.hpp>
#include <vnigma/message/message_variant.hpp>
#include <vnigma/util/binary.hpp>

#include "mock_devices.hpp"

namespace vn {
using namespace vnigma;
using vnigma::das::set_config;
}  // namespace vn

using std::optional;
using std::tuple;
using std::variant;
using namespace vnigma::literals;

using initv = variant<vn::set_config::port_t, uint8_t, tuple<uint8_t, uint8_t>>;

struct das_sc_p {
  vn::uuid uid;
  vn::buffer buf;
  vn::device dev;
  initv init;
  optional<vn::system_error> error;
};

class SetConfigTest : public ::testing::TestWithParam<das_sc_p> {
 public:
  vn::set_config make_cmd() {
    auto param = GetParam();
    if (std::holds_alternative<vn::set_config::port_t>(param.init)) {
      auto port = std::get<vn::set_config::port_t>(param.init);
      return vn::set_config(param.uid, param.dev, port);
    }
    if (std::holds_alternative<uint8_t>(param.init)) {
      uint8_t index = std::get<uint8_t>(param.init);
      return vn::set_config(param.uid, param.dev, index);
    }
    if (std::holds_alternative<tuple<uint8_t, uint8_t>>(param.init)) {
      tuple<uint8_t, uint8_t> p = std::get<tuple<uint8_t, uint8_t>>(param.init);
      uint8_t index = std::get<0>(p);
      uint8_t config = std::get<1>(p);
      return vn::set_config(param.uid, param.dev, index, config);
    }

    throw std::runtime_error("invalid port init param");
  }
};

TEST_F(SetConfigTest, traits) {
  EXPECT_TRUE(vn::is_command<vn::set_config>());
  EXPECT_FALSE(vn::is_response<vn::set_config>());

  EXPECT_TRUE(vn::das_related<vn::set_config>());
  EXPECT_FALSE(vn::venom_related<vn::set_config>());

  EXPECT_TRUE(vn::has_payload<vn::set_config>());

  EXPECT_FALSE(vn::is_port_missed<vn::set_config>());
  EXPECT_TRUE(vn::is_port_scoped<vn::set_config>());

  EXPECT_FALSE(vn::is_data<vn::set_config>());
}

TEST_P(SetConfigTest, as_buffer) {
  auto param = GetParam();
  if (param.error.has_value()) {
    GTEST_SKIP() << "Skipped cause should fail";
  }

  vn::set_config cmd = make_cmd();
  auto buf = cmd.as_buffer();

  EXPECT_EQ(buf, param.buf) << buf << " is not equal to " << param.buf;
}

TEST_P(SetConfigTest, from_buffer) {
  auto param = GetParam();
  if (param.error.has_value()) {
    GTEST_SKIP() << "Skipped cause should fail";
  }

  vn::set_config cmd(param.buf);
  auto buf = cmd.as_buffer();

  EXPECT_EQ(buf, param.buf) << buf << " is not equal to " << param.buf;
}

INSTANTIATE_TEST_SUITE_P(
    DasSetConfig, SetConfigTest,
    ::testing::Values(
        das_sc_p{232, "<DSASC,232,1,1,F7\r\n"_mb, mock::f_analog(), 1,
                 std::nullopt},
        das_sc_p{232, "<DSASC,232,1,4,F2\r\n"_mb, mock::f_analog(),
                 vn::serial::port(4, vn::binary<11110010>::value),
                 std::nullopt},
        das_sc_p{232, "<DSASC,232,1,2,F7\r\n"_mb, mock::f_analog(),
                 vn::serial::port(2, vn::binary<11110111>::value),
                 std::nullopt},
        das_sc_p{
            232, "<DSASC,232,1,1,F7\r\n"_mb, mock::f_analog(),
            std::make_tuple<uint8_t, uint8_t>(1, vn::binary<11110111>::value),
            std::nullopt}));
