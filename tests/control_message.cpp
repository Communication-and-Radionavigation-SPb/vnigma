#include <gtest/gtest.h>

#include <string>

#include <vnigma/message/das_get_config.h>
#include <vnigma/message/das_set_frequency.h>
#include <vnigma/buffer.hpp>
#include <vnigma/message/control_message.hpp>

#define Suite ControlMessageTest

namespace vn = vnigma;
using namespace vn::literals;

TEST(Suite, das_gc_from_buffer) {
  vn::buffer buf = "<DSSGC,100,1\r\n"_mb;
  vn::control_message<vn::das::get_config> msg(buf);
}

TEST(Suite, das_sf_from_buffer) {
  vn::buffer buf = "<DSASF,100,1,,-1\r\n";
  vn::control_message<vn::das::set_frequency> msg(buf);
}

template <typename T>
class DasFailsTest : public ::testing::Test {};
TYPED_TEST_SUITE_P(DasFailsTest);

TYPED_TEST_P(DasFailsTest, empty) {
  vn::buffer buf = ""_mb;
  EXPECT_THROW({ vn::control_message<TypeParam> msg(buf); }, vn::system_error);
}

TYPED_TEST_P(DasFailsTest, no_prefix) {
  vn::buffer buf = "\r\n"_mb;
  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "wrong prefix: Bad message";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << error << " do not contains " << match;
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, no_lf) {
  vn::buffer buf = "<\r"_mb;

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "no line ending: Bad message";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << error << " do not contains " << match;
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, no_cr) {
  vn::buffer buf = "<\n"_mb;

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "no carret return: Bad message";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << error << " do not contains " << match;
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, no_protocol) {
  vn::buffer buf = "<\r\n"_mb;

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "no target protocol: Protocol error";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, protocol_missmatch) {
  vn::buffer buf = "<**\r\n"_mb;

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "invalid protocol version **: Protocol error";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

REGISTER_TYPED_TEST_SUITE_P(DasFailsTest, empty, no_prefix, no_lf, no_cr,
                            no_protocol, protocol_missmatch);

using DasMsgTypes =
    ::testing::Types<vn::das::get_config, vn::das::set_frequency>;
INSTANTIATE_TYPED_TEST_SUITE_P(Das, DasFailsTest, DasMsgTypes);