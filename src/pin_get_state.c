
#include "pin_daemon_internal.h"

PinState get_state(
    const PinInConfiguration* pin_in) {
  // TODO still haven't tried this functionality out
  // TODO need to handle the case where it fails to open
  FILE* f = fopen(pin_in->value_path, "r");
  drain_sysfs_file(f);
  fclose(f);
}


