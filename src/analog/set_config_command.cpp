#include <vnigma/analog/set_config_command.h>
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <sstream>

namespace vnigma { namespace analog { namespace commands {
SetConfigurationCommand::SetConfigurationCommand(
    core::Module* a_module, core::GuidGenerator* a_generator,
    std::vector<Port> a_ports)
    : Command(a_module, a_generator), ports(a_ports) {
  assert(ports.size() == ANALOG_PORTS_COUNT);
  std::sort(ports.begin(), ports.end(),
            [](const Port& left, const Port& right) {
              return left.index < right.index;
            });
}

std::string SetConfigurationCommand::get_name() const noexcept {
  return "SC";
}

std::string SetConfigurationCommand::get_payload() const noexcept {
  uint16_t pl = 0;
  auto it = ports.begin();
  auto end = ports.end();
  while (it != end) {
    pl += it->as_hex() << (it->index * 2);
    it++;
  }

  std::stringstream ss;
  ss << ",";
  ss << std::hex << std::uppercase << (unsigned int)pl;

  return ss.str();
}
}}}  // namespace vnigma::analog::commands
