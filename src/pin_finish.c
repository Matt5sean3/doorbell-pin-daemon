
#include "pin_daemon_internal.h"

int pin_finish(const PinConfiguration* configuration) {
  int i;

  // unexport input pins
  for(i = 0; i < configuration->num_pin_ins; i++)
    write_to_file(
        configuration->unexport_path,
        configuration->pin_ins[i].identifier);

  // unexport output pins
  for(i = 0; i < configuration->num_pin_outs; i++)
    write_to_file(
        configuration->unexport_path,
        configuration->pin_outs[i].identifier);
}

