#pragma once

#include <vector>
#include <vnigma/export.h>
#include <vnigma/command.h>
#include <vnigma/serial/serial_port.h>

namespace vnigma { namespace serial { namespace commands {

#define SERIAL_PORTS_NUMBER 8

class VNIGMA_EXPORT SetConfigurationCommand : public core::Command<SetConfigurationCommand> {
 private:
  std::vector<Port> ports_;

 public:
  SetConfigurationCommand(core::Module* a_module,
                          core::GuidGenerator* a_generator,
                          std::vector<Port> ports);

 public:
  std::string get_name() const noexcept;

  std::string get_payload() noexcept;
};



}}}  // namespace vnigma::serial::commands
