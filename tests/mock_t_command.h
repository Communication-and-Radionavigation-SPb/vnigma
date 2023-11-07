#pragma once

#include <vnigma/command.h>

class MockTCommand : public vnigma::core::Command<MockTCommand> {
 private:
  std::string payload;

 public:
  MockTCommand(vnigma::core::Module* a_module,
               vnigma::core::GuidGenerator* a_generator, std::string payload)
      : Command<MockTCommand>(a_module, a_generator), payload(payload) {}

 public:
  std::string get_name() const noexcept { return "MT"; }

  std::string get_payload() const noexcept { return payload; }
};