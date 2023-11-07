#include <vnigma/serial/set_config_command.h>
#include <algorithm>

namespace vnigma { namespace serial { namespace commands {
/* -------------------------------------------------------------------------- */
SetConfigurationCommand::SetConfigurationCommand(
    core::Module* a_module, core::GuidGenerator* a_generator,
    std::vector<Port> ports)
    : Command(a_module, a_generator), ports_(ports) {
  // Sort passed ports
  std::sort(ports_.begin(), ports_.end(),
            [](const Port& left, const Port& right) {
              return left.index < right.index;
            });
}
/* -------------------------------------------------------------------------- */
std::string SetConfigurationCommand::get_name() const noexcept {
  return "SC";
}
/* -------------------------------------------------------------------------- */
std::string SetConfigurationCommand::get_payload() noexcept {
  std::stringstream ss;

  auto it = ports_.begin();

  // Loop over max possible ports configuration
  for (size_t i = 0; i < SERIAL_PORTS_NUMBER; i++) {
    // If we have such port passed
    if (it->index == i) {
      // Add its configuration
      ss << std::uppercase << std::hex << (int)it->as_hex();
      ++it;
    }
    
    // If not last
    if (i < SERIAL_PORTS_NUMBER - 1) {
      ss << ',';
    }
  }

  return ss.str();
}
/* -------------------------------------------------------------------------- */
}}}  // namespace vnigma::serial::commands
