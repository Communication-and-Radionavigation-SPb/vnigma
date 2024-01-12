#include <gtest/gtest.h>

#include <string>

#include <vnigma/message/das_get_config.h>
#include <vnigma/message/das_greed_send_data.h>
#include <vnigma/message/das_handshake.h>
#include <vnigma/message/das_scoped_send_data.h>
#include <vnigma/message/das_set_config.h>
#include <vnigma/message/das_set_frequency.h>
#include <vnigma/message/das_set_reset.h>
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
          std::string match = "invalid protocol version '': Protocol error";
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
          std::string match = "invalid protocol version '**': Protocol error";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, device_clarifier) {
  vn::buffer buf = "<DS\r\n"_mb;
  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "no device clarifier: Protocol error";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, device_unknown) {
  vn::buffer buf = "<DS*\r\n"_mb;
  EXPECT_THROW({ vn::control_message<TypeParam> msg(buf); }, vn::system_error);
}

TYPED_TEST_P(DasFailsTest, format_token_apsent) {
  std::stringstream ss;
  ss << "<DS";
  if constexpr (vn::is_service<TypeParam>()) {
    ss << "X";
  }
  else {
    ss << "S";
  }
  ss << "\r\n";
  std::string msg = ss.str();
  vn::buffer buf(msg.c_str());

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "format token apsent: Bad message";
          EXPECT_EQ(e.code(), errc::bad_message);
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, format_token_not_matches) {
  std::stringstream ss;
  ss << "<DS";

  if constexpr (vn::is_service<TypeParam>()) {
    ss << "X";
  }
  else {
    ss << "S";
  }
  ss << "**\r\n";
  std::string msg = ss.str();

  vn::buffer buf(msg.c_str());
  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match =
              "message format token not matches target one: Bad message";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, no_uid) {
  std::string fmt = vn::control_str<TypeParam>::value;
  if constexpr (!vn::is_command<TypeParam>()) {
    GTEST_SKIP() << "Skipeed cause not command";
  }
  std::stringstream ss;
  ss << "<DSS" << fmt << ","
     << "\r\n";
  std::string payload = ss.str();
  vn::buffer buf(payload.c_str());

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "uuid apsent: Bad message";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, invalid_uid) {
  std::stringstream ss;
  std::string fmt = vn::control_str<TypeParam>::value;
  if constexpr (!vn::is_command<TypeParam>()) {
    GTEST_SKIP() << "Skipeed cause " << fmt << " is not command";
  }
  ss << "<DSS" << fmt << ",*"
     << "\r\n";
  std::string payload = ss.str();
  vn::buffer buf(payload.c_str());

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "uuid invalid: Bad message";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, no_device_id) {
  if (vn::is_service<TypeParam>) {
    GTEST_SKIP();
  }

  std::stringstream ss;
  std::string fmt = vn::control_str<TypeParam>::value;
  ss << "<DSS" << fmt;
  if constexpr (vn::is_command<TypeParam>()) {
    ss << ",242";
  }
  ss << "\r\n";
  std::string payload = ss.str();
  vn::buffer buf(payload.c_str());

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "device identifier apsent: Bad message";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, device_id_empty) {
  if (vn::is_service<TypeParam>()) {
    GTEST_SKIP();
  }
  std::stringstream ss;
  std::string fmt = vn::control_str<TypeParam>::value;
  ss << "<DSS" << fmt;
  if constexpr (vn::is_command<TypeParam>()) {
    ss << ",242";
  }
  ss << ","
     << "\r\n";
  std::string payload = ss.str();
  vn::buffer buf(payload.c_str());

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "device identifier empty: Bad message";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, device_id_invalid) {
  if (vn::is_service<TypeParam>()) {
    GTEST_SKIP();
  }
  std::stringstream ss;
  std::string fmt = vn::control_str<TypeParam>::value;

  ss << "<DSS" << fmt;
  if constexpr (vn::is_command<TypeParam>()) {
    ss << ",242";
  }
  ss << ",*"
     << "\r\n";
  std::string payload = ss.str();
  vn::buffer buf(payload.c_str());

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "device identifier invalid: Bad message";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, no_port_field) {
  if constexpr (!vn::is_port_missed<TypeParam>() &&
                !vn::is_port_scoped<TypeParam>()) {

    GTEST_SKIP() << "Skipped due to unrelated to port";
  }
  std::stringstream ss;
  std::string fmt = vn::control_str<TypeParam>::value;
  ss << "<DSS" << fmt;
  if constexpr (vn::is_command<TypeParam>()) {
    ss << ",242";
  }
  ss << ",1"
     << "\r\n";
  std::string payload = ss.str();
  vn::buffer buf(payload.c_str());

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "port field missed: Bad message";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, port_field_empty) {
  if constexpr (!vn::is_port_scoped<TypeParam>()) {
    GTEST_SKIP() << "Skipped due to unrelated to port";
  }
  std::stringstream ss;
  std::string fmt = vn::control_str<TypeParam>::value;
  ss << "<DSS" << fmt;
  if constexpr (vn::is_command<TypeParam>()) {
    ss << ",242";
  }
  ss << ",1,"
     << "\r\n";
  std::string payload = ss.str();
  vn::buffer buf(payload.c_str());

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "port field empty: Bad message";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, port_missed_field_invalid) {
  if constexpr (!vn::is_port_missed<TypeParam>()) {
    GTEST_SKIP() << "Skipped due to unrelated to port missed";
  }
  std::stringstream ss;
  std::string fmt = vn::control_str<TypeParam>::value;
  ss << "<DSS" << fmt;
  if constexpr (vn::is_command<TypeParam>()) {
    ss << ",242";
  }
  ss << ",1,_";
  //        ^
  //  port missed fields
  // should not contain anything
  //    inside port field
  ss << "\r\n";
  std::string payload = ss.str();
  vn::buffer buf(payload.c_str());

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "invalid port field: Bad message";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, port_scoped_field_invalid) {
  if constexpr (!vn::is_port_scoped<TypeParam>()) {
    GTEST_SKIP() << "Skipped due to unrelated to port scoped";
  }
  std::stringstream ss;
  std::string fmt = vn::control_str<TypeParam>::value;
  ss << "<DSS" << fmt;
  if constexpr (vn::is_command<TypeParam>()) {
    ss << ",242";
  }
  ss << ",1,*";
  //        ^
  //  port scoped command expects field to contain number
  ss << "\r\n";
  std::string payload = ss.str();
  vn::buffer buf(payload.c_str());

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "invalid port field: Bad message";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, payload_apsent) {
  if constexpr (!vn::has_payload<TypeParam>()) {
    GTEST_SKIP() << "Skipped cause not contains payload";
  }
  std::stringstream ss;
  std::string fmt = vn::control_str<TypeParam>::value;
  std::string port = "";
  if constexpr (vn::is_port_missed<TypeParam>() ||
                vn::is_port_scoped<TypeParam>()) {
    port = ",";
  }
  if constexpr (vn::is_port_scoped<TypeParam>()) {
    port += "1";
  }
  ss << "<DS";
  if constexpr (vn::is_service<TypeParam>()) {
    ss << "X";
  }
  else {
    ss << "S";
  }
  ss << fmt;
  if constexpr (vn::is_command<TypeParam>()) {
    ss << ",242";
  }
  if constexpr (!vn::is_service<TypeParam>()) {
    ss << ",1";
  }
  ss << port << "\r\n";
  std::string payload = ss.str();
  vn::buffer buf(payload.c_str());

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "payload apsent: Bad message";
          EXPECT_TRUE(error.find(match) != std::string::npos)
              << "'" << error << "'"
              << " do not contains "
              << "'" << match << "'";
          throw;
        }
      },
      vn::system_error);
}

TYPED_TEST_P(DasFailsTest, payload_empty) {
  if constexpr (!vn::has_payload<TypeParam>()) {
    GTEST_SKIP() << "Skipped cause not contains payload";
  }
  std::stringstream ss;
  std::string fmt = vn::control_str<TypeParam>::value;
  std::string port = "";

  if constexpr (vn::is_port_missed<TypeParam>() ||
                vn::is_port_scoped<TypeParam>()) {
    port = ",";
  }
  if constexpr (vn::is_port_scoped<TypeParam>()) {
    port += "1";
  }
  ss << "<DS";
  if constexpr (vn::is_service<TypeParam>()) {
    ss << "X";
  }
  else {
    ss << "S";
  }
  ss << fmt;
  if constexpr (vn::is_command<TypeParam>()) {
    ss << ",242";
  }
  if constexpr (!vn::is_service<TypeParam>()) {
    ss << ",1";
  }
  ss << port << ","
     << "\r\n";
  std::cout << ss.str() << std::endl;
  std::string payload = ss.str();
  vn::buffer buf(payload.c_str());

  EXPECT_THROW(
      {
        try {
          vn::control_message<TypeParam> msg(buf);
        } catch (const vn::system_error& e) {
          std::string error = e.what();
          std::string match = "payload empty: Bad message";
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
                            no_protocol, protocol_missmatch, device_clarifier,
                            device_unknown, format_token_apsent,
                            format_token_not_matches, no_uid, invalid_uid,
                            no_device_id, device_id_empty, device_id_invalid,
                            no_port_field, port_field_empty,
                            port_scoped_field_invalid,
                            port_missed_field_invalid, payload_apsent,
                            payload_empty);

using DasMsgTypes =
    ::testing::Types<vn::das::get_config, vn::das::set_frequency,
                     vn::das::set_config, vn::das::set_reset,
                     vn::das::greed_send_data, vn::das::scoped_send_data,
                     vn::das::handshake>;
INSTANTIATE_TYPED_TEST_SUITE_P(Das, DasFailsTest, DasMsgTypes);