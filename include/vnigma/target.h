#pragma once

#include <vnigma/export.h>
#include <string>

namespace vnigma { namespace core {
VNIGMA_EXPORT enum Target { vn, das };

/**
 * @brief Converts <vnigma::core::Target> to its string
 * representation
 * 
 * @param tgt Target to convert
 * @return std::string String representation of target device
 */
std::string target_to_string(const Target& tgt) noexcept;


/**
 * @brief Resolves which <vnigma::core::Target> is
 * encoded in passed string param
 * 
 * @param str String which potentially contains target device type
 * @throws If passed string do not matches any known target device type
 * @return Target Resolved target device type
 */
Target target_from_string(const std::string& str);
}} // namespace vnigma::core
