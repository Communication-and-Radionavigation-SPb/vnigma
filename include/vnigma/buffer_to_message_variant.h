#if !defined(VNIGMA_BUFFER_TO_MESSAGE_VARIANT)
#define VNIGMA_BUFFER_TO_MESSAGE_VARIANT

#include <vnigma/buffer.hpp>
#include <vnigma/message/message_variant.hpp>

namespace vnigma {
message_variant buffer_to_message_variant(buffer buf);
}  // namespace vnigma

#endif  // VNIGMA_BUFFER_TO_MESSAGE_VARIANT
