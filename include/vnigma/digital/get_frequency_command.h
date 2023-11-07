#pragma once

#include <vnigma/command.h>

namespace vnigma { namespace digital { namespace commands {
class GetFrequencyCommand : public core::Command<GetFrequencyCommand> {
 public:
  GetFrequencyCommand(core::Module*, core::GuidGenerator*);

 public:
  std::string get_name() const noexcept;

  std::string get_payload() const noexcept;
};
}}}  // namespace vnigma::digital::commands
