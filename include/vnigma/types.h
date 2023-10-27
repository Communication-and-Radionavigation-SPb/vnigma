#pragma once

#include <vnigma/export.h>
#include <string>
#include <stdexcept>

namespace vnigma { namespace core {

/**
 * @brief Module type
 * 
 */
VNIGMA_EXPORT enum Type { analog, serial, digital };

/**
 * @brief Converts module <Type> to string which
 * contains module type letter
 * 
 * @param t Type of module
 * @return std::string String representation of module type
 */
std::string type_to_string(const Type& t) noexcept;

/**
 * @brief Converts string representation of module
 * type into <vnigma::core::Type> type
 * 
 * @param str String which contains module type
 * @throws std::invalid_argument if passed parameter do not
 * contains any known module type
 * @return Type Type of module
 */
Type type_from_string(const std::string& str);
}}  // namespace vnigma::core
