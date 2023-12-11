#include <vnigma/device.h>

namespace vnigma {

device::device(id_t id, Type type) : id_(id), type_(type) {}

id_t device::id() const noexcept {
  return id_;
}

Type device::type() const noexcept {
  return type_;
}

}  // namespace vnigma
