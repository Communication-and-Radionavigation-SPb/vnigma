#pragma once

#include <vnigma/command.h>
#include <vnigma/export.h>

namespace vnigma { namespace commands { namespace base {

class VNIGMA_EXPORT ResetCommand : public core::Command<ResetCommand> {
 public:
  ResetCommand(core::Module* a_module, core::GuidGenerator* a_generator)
      : Command(a_module, a_generator) {}

 public:
  std::string get_name() const noexcept { return "SR"; }

  std::string get_payload() const noexcept { return ""; }
};

}}}  // namespace vnigma::commands::base
