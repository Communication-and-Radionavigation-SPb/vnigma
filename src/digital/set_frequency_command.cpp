#include <vnigma/digital/set_frequency_command.h>
#include <cassert>
#include <sstream>

namespace vnigma { namespace digital { namespace commands {
SetFrequencyCommand::SetFrequencyCommand(core::Module* a_module,
                                         core::GuidGenerator* a_generator,
                                         data::Period period)
    : Command(a_module, a_generator), get_data_period(period) {
  assert(module_->type() == core::digital);
}

std::string SetFrequencyCommand::get_name() const noexcept {
  return "SF";
}

std::string SetFrequencyCommand::get_payload() const noexcept {
  std::stringstream ss;
  ss << ",";
  ss << data::to_string(get_data_period);
  return ss.str();
}

}}}  // namespace vnigma::digital::commands
