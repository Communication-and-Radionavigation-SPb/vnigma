#pragma once

#include <vnigma/command.h>
#include <vnigma/digital/types.h>
#include <vnigma/export.h>
#include <optional>

namespace vnigma { namespace digital { namespace commands {
class VNIGMA_EXPORT SetFrequencyCommand
    : public core::Command<SetFrequencyCommand> {
 public:
  data::Period get_data_period;

 public:
  SetFrequencyCommand(core::Module*, core::GuidGenerator*, data::Period);

 public:
  std::string get_name() const noexcept;

  std::string get_payload() const noexcept;
};
}}}  // namespace vnigma::digital::commands
