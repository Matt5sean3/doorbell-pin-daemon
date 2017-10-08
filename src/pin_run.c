
#include "pin_daemon_internal.h"

// Node types for linked lists
typedef struct InputPinNode InputPinNode;
typedef struct OutputPinNode OutputPinNode;

int pin_run(const PinConfiguration* configuration) {

  int success;
  int i;

  // Create the set of polled pins
  struct pollfd poll_fds[configuration->num_pin_ins];
  FILE* pin_in_file_handles[configuration->num_pin_ins];

  pin_start(configuration);


  for(i = 0; i < configuration->num_pin_ins; i++) {
    pin_in_file_handles[i] = fopen(configuration->pin_ins[i].value_path, "r");
  }

  for(i = 0; i < configuration->num_pin_ins; i++) {
    fclose(pin_in_file_handles[i]);
  }

  pin_finish(configuration);

}

