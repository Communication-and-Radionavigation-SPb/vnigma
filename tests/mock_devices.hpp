#include <vnigma/device.h>

namespace mock {
const auto f_analog = []() -> vnigma::device {
  return vnigma::device(1, vnigma::core::analog);
};
const auto f_serial = []() -> vnigma::device {
  return vnigma::device(1, vnigma::core::serial);
};

const auto f_digital = []() -> vnigma::device {
  return vnigma::device(1, vnigma::core::digital);
};
}  // namespace mock
