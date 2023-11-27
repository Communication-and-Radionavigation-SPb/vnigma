#pragma once

/* ----------------------------------- STD ---------------------------------- */
#include <cstdint>
/* -------------------------------- Internal -------------------------------- */
#include <vnigma/export.h>
#include <vnigma/id_t.h>

namespace vnigma {
struct VNIGMA_EXPORT guid {
  static uuid next() noexcept {
    static uuid cuid = 0;
    return cuid++;
  }
};

}  // namespace vnigma
