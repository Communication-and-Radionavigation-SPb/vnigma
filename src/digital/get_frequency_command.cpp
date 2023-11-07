#include <vnigma/digital/get_frequency_command.h>

namespace vnigma { namespace digital { namespace commands {
GetFrequencyCommand::GetFrequencyCommand(core::Module* a_module,
                                         core::GuidGenerator* generator)
    : Command(a_module, generator) {}

std::string GetFrequencyCommand::get_name() const noexcept {
  return "GF";
}

std::string GetFrequencyCommand::get_payload() const noexcept {
  return "";
}

}}}  // namespace vnigma::digital::commands
