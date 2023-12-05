#if !defined(VNIGMA_BUFFER_TO_MESSAGE_VARIANT)
#define VNIGMA_BUFFER_TO_MESSAGE_VARIANT

#include <vnigma/export.h>
#include <vnigma/buffer.hpp>
#include <vnigma/message/message_variant.hpp>

namespace vnigma {

/**
 * @brief returns protocol from buffer
 */
std::string_view VNIGMA_EXPORT get_protocol(buffer& buf);

/**
 * @brief returns control string from buffer
 */
std::string_view VNIGMA_EXPORT get_control(buffer& buf);

/**
 * @brief converts buffer to message variant
 * 
 * @param buf buffer to parse
 * @return message_variant one of messages
 */
message_variant VNIGMA_EXPORT buffer_to_message_variant(buffer buf);
}  // namespace vnigma

#endif  // VNIGMA_BUFFER_TO_MESSAGE_VARIANT
