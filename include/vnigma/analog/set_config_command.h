#pragma once

#include <vnigma/analog/port.h>
#include <vnigma/command.h>
#include <vector>

namespace vnigma { namespace analog { namespace commands {
class VNIGMA_EXPORT SetConfigurationCommand
    : public core::Command<SetConfigurationCommand> {
 public:
  std::vector<Port> ports;

 public:
  SetConfigurationCommand(core::Module* a_module,
                          core::GuidGenerator* a_genertor,
                          std::vector<Port> a_ports);

 public:
  std::string get_name() const noexcept;

  std::string get_payload() const noexcept;
};
}}}  // namespace vnigma::analog::commands
