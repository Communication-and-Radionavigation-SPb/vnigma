#include <gtest/gtest.h>

#include <vnigma/message/das_set_frequency.h>
#include <optional>
#include <tuple>
#include <type_traits>
#include <vnigma/buffer.hpp>
#include <vnigma/message/message_traits.hpp>

#define Suite SetFrequencyTest

namespace vn {
using namespace vnigma;
using vnigma::das::set_frequency;
}  // namespace vn

using std::make_tuple;
using std::optional;
using std::tuple;

using namespace vn::literals;

struct set_frequency_tp {
  vn::buffer buf;
  vn::uuid uid;
  vnigma::core::send_period period;
  vnigma::device dev;
  optional<tuple<errc, std::string>> err;
};

class SetFrequencyTest : public ::testing::TestWithParam<set_frequency_tp> {};

TEST_F(Suite, traits) {
  EXPECT_TRUE(vn::is_command<vn::set_frequency>());
  EXPECT_FALSE(vn::is_response<vn::set_frequency>());

  EXPECT_TRUE(vn::das_related<vn::set_frequency>());
  EXPECT_FALSE(vn::venom_related<vn::set_frequency>());

  EXPECT_TRUE(vn::has_payload<vn::set_frequency>());

  EXPECT_TRUE(vn::is_port_missed<vn::set_frequency>());
  EXPECT_FALSE(vn::is_port_scoped<vn::set_frequency>());

  EXPECT_FALSE(vn::is_data<vn::set_frequency>());
}

TEST_P(Suite, as_buffer) {
  auto param = GetParam();
  if (param.err.has_value()) {
    GTEST_SKIP() << "Skipped cause should be tested as failure";
  }
  vn::set_frequency cmd(param.uid, param.dev, param.period);
  auto buf = cmd.as_buffer();

  EXPECT_EQ(buf, param.buf) << buf << " is not eq to " << param.buf;
}

TEST_P(Suite, from_buffer) {
  auto param = GetParam();
  if (param.err.has_value()) {
    GTEST_SKIP() << "Skipped cause should be tested as failure";
  }

  vn::set_frequency cmd(param.buf);
  auto buf = cmd.as_buffer();
  EXPECT_EQ(buf, param.buf);
}

TEST_P(Suite, from_buffer_fails) {
  auto param = GetParam();
  if (!param.err.has_value()) {
    GTEST_SKIP() << "Skipped cause should not fail";
  }

  auto thr = [&]() {
    try {
      vn::set_frequency cmd(param.buf);
    } catch (const vn::system_error& e) {
      std::string error = e.what();
      EXPECT_EQ(e.code(), std::get<0>(param.err.value()));
      EXPECT_EQ(error, std::get<1>(param.err.value()));
      throw;
    }
  };

  EXPECT_THROW({ thr(); }, vn::system_error);
}

INSTANTIATE_TEST_SUITE_P(  // instantiate
    SequencePeriod,        // suite
    Suite,                 // cases
    ::testing::Values(     // values begin
        set_frequency_tp{"<DSASF,100,0,,-1\r\n"_mb, 100, vn::core::never,
                         vn::device(0, vn::core::analog), std::nullopt},
        set_frequency_tp{"<DSDSF,100,1,,500\r\n"_mb, 100, vn::core::fast,
                         vn::device(1, vn::core::digital), std::nullopt},
        set_frequency_tp{"<DSASF,100,2,,1000\r\n"_mb, 100, vn::core::normal,
                         vn::device(2, vn::core::analog), std::nullopt},
        set_frequency_tp{"<DSASF,100,3,,2000\r\n"_mb, 100, vn::core::slow,
                         vn::device(3, vn::core::analog), std::nullopt},
        set_frequency_tp{"<DSASF,100,4,,5000\r\n"_mb, 100, vn::core::slower,
                         vn::device(4, vn::core::analog), std::nullopt},
        set_frequency_tp{"<DSASF,100,5,,10000\r\n"_mb, 100, vn::core::slowest,
                         vn::device(5, vn::core::analog), std::nullopt},
        set_frequency_tp{"<DSASF,100,5,,*\r\n"_mb, 100, vn::core::slowest,
                         vn::device(5, vn::core::analog),
                         std::make_tuple(errc::bad_message,
                                         "vnigma::das::set_frequency invalid "
                                         "frequency value: Bad message")}
        // values end
        ));
