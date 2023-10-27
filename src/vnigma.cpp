#include <vnigma/vnigma.h>

namespace vnigma {
namespace core {
Module::Module(uint8_t a_id, Target a_target, Type a_type)
    : id(a_id), target(a_target), type(a_type) {}
}  // namespace core

}  // namespace vnigma
