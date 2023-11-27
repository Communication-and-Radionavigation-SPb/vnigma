#include <vnigma/device.h>

namespace vnigma {

device::device(uint8_t id, Type type) : id_(id), type_(type) {}

uint8_t device::id() const noexcept {
  return id_;
}

Type device::type() const noexcept {
  return type_;
}

}  // namespace vnigma
