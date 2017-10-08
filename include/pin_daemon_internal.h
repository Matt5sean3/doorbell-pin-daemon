
#ifndef PIN_DAEMON_INTERNAL_H_
#define PIN_DAEMON_INTERNAL_H_

#include "pin_daemon.h"
#include <stdio.h>
#include <poll.h>

struct PinIn {
  PinInConfiguration* configuration;
};

struct PinOut {
  PinOutConfiguration* configuration;
};

// Used internally, but not meant for external use
struct PinContext {

  int num_pin_ins;
  PinIn* pin_ins;

  int num_pin_outs;
  PinOut* pin_outs;

};

int write_to_file(const char* path, const char* text);

#endif

