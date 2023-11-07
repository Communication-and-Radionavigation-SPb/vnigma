#include <vnigma/analog/set_frequency_command.h>
#include <sstream>

namespace vnigma { namespace analog { namespace commands {
SetFrequencyCommand::SetFrequencyCommand(core::Module* a_module,
                                         core::GuidGenerator* a_generator,
                                         opt<data::Period> a_data,
                                         opt<config::Period> a_config)
    : Command(a_module, a_generator),
      get_data_period(a_data),
      get_config_period(a_config) {}

std::string SetFrequencyCommand::get_name() const noexcept {
  return "SF";
}

std::string SetFrequencyCommand::get_payload() const noexcept {
  std::stringstream ss;
  ss << ",";
  if (get_data_period.has_value()) {
   ss << data::to_string(get_data_period.value()); 
  }
  ss << ",";

  if (get_config_period.has_value()) {
    ss << config::to_string(get_config_period.value());
  }

  return ss.str();
}
}}}  // namespace vnigma::analog::commands
