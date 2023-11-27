#if !defined(VNIGMA_BUFFER_TO_DATA_VARIANT_H)
#define VNIGMA_BUFFER_TO_DATA_VARIANT_H

#include <vnigma/buffer.hpp>
#include <vnigma/data.hpp>

namespace vnigma {
  data_variant buffer_to_data_variant(buffer buf);
}  // namespace vnigma

#endif  // VNIGMA_BUFFER_TO_DATA_VARIANT_H
