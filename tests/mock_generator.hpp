#pragma once

#include <vnigma/guid_generator.h>

class MockGuidGenerator : public vnigma::core::GuidGenerator {
 public:
  unsigned int next() const noexcept override { return 1; }
};