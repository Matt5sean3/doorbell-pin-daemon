
#include "pin_daemon_internal.h"

int pin_set_state(
    const PinOutConfiguration* pin_out,
    PinState state) {
  write_to_file(
      pin_out->value_path,
      state == LOW ? "0" :
      state == HIGH ? "1" :
      "0");
}

