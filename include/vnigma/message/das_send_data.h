#if !defined(VNIGMA_MESSAGE_ANALOG_SEND_DATA_H)
#define VNIGMA_MESSAGE_ANALOG_SEND_DATA_H

/* ----------------------------------- STD ---------------------------------- */
#include <optional>
/* -------------------------------- Internal -------------------------------- */
#include <vnigma/export.h>
#include <vnigma/message/das_message.h>
#include <vnigma/buffer.hpp>
#include <vnigma/data.hpp>
#include <vnigma/message/control_message.hpp>

/* -------------------------------------------------------------------------- */
/*                             Send data response                             */
/* -------------------------------------------------------------------------- */

namespace vnigma {
/* --------------------------- Send data response --------------------------- */
namespace das {
using std::optional;
/**
 * @brief base class for all send data messages
 * should not be used in real code
 */
class VNIGMA_EXPORT send_data {
 protected:
  send_data(data_variant data);
  send_data(buffer buf, Type target_type);

 public:
  virtual std::string payload() const;

 protected:
  data_variant get_data() const;

 private:
  optional<data_variant> var_;
};

}  // namespace das
}  // namespace vnigma

#endif  // VNIGMA_MESSAGE_ANALOG_SEND_DATA_H
