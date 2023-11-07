#pragma once

#include <vnigma/export.h>

namespace vnigma { namespace serial {

VNIGMA_EXPORT enum Baud {
  Baud2400 = 1,
  Baud4800 = 2,
  Baud9600 = 3,
  Baud19200 = 4,
  Baud38400 = 5,
  Baud115200 = 6
};

VNIGMA_EXPORT enum Iface {
  RS232 = 0,
  RS422 = 1,
};

VNIGMA_EXPORT enum State { Enabled = true, Disabled = false };

}}  // namespace vnigma::serial
