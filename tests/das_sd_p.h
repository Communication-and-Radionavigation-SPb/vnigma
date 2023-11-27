#if !defined(VNIGMA_TESTS_DAS_SD_P_H)
#define VNIGMA_TESTS_DAS_SD_P_H

#include <vnigma/device.h>
#include <vnigma/buffer.hpp>
#include <vnigma/data.hpp>

namespace vn {
using namespace vnigma;
}

struct das_sd_p {
  vn::buffer buf;
  vn::device dev;
  vn::data_variant data;
};

#endif  // VNIGMA_TESTS_DAS_SD_P_H
