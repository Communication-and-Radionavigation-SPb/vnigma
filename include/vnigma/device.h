#if !defined(VNIGMA_DEVICE_H)
#define VNIGMA_DEVICE_H
/* ----------------------------------- STD ---------------------------------- */
#include <cstdint>
/* -------------------------------- Internal -------------------------------- */
#include <vnigma/types.h>
/* -------------------------------------------------------------------------- */
/*                            Vnigma target device                            */
/* -------------------------------------------------------------------------- */
namespace vnigma {
/* ---------------------------------- Using --------------------------------- */
using namespace core;

/**
 * @brief Vnigma module device
 */
class device {
 public:
  using id_t = uint32_t;

 private:
  id_t id_;
  Type type_;

 public:
  /**
   * @brief constructor 
   * 
   * @param id module identifier
   * @param type module type @link vnigma::core::Type
   */
  device(id_t id, Type type);

 public:
  /**
  * @brief device id
  * @return uint32_t id
  */
  id_t id() const noexcept;

  /**
  * @brief device type
  * @return Type type
  */
  Type type() const noexcept;

 public:
  bool operator==(const device& other) const {
    return id_ == other.id_ && type_ == other.type_;
  }
};
}  // namespace vnigma

#endif  // VNIGMA_DEVICE_H
