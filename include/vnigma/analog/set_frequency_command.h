#pragma once

#include <vnigma/export.h>
#include <vnigma/analog/types.h>
#include <vnigma/command.h>
#include <optional>

namespace vnigma { namespace analog { namespace commands {

template <typename T>
using opt = std::optional<T>;

class SetFrequencyCommand : public core::Command<SetFrequencyCommand> {
 public:
  opt<data::Period> get_data_period;
  opt<config::Period> get_config_period;

 public:
  SetFrequencyCommand(core::Module*, core::GuidGenerator*,
                      opt<data::Period> = std::nullopt,
                      opt<config::Period> = std::nullopt);

 public:
  std::string get_name() const noexcept;

  std::string get_payload() const noexcept;
};

}}}  // namespace vnigma::analog::commands
