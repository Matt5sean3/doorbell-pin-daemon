
#include "pin_daemon_internal.h"

// Uses `wait_for_file` to allow permission changes to propagate after input to export

int pin_start(const PinConfiguration* configuration) {
  int i;

  for(i = 0; i < configuration->num_pin_ins; i++) {
    // export the pin
    write_to_file(
        configuration->export_path,
        configuration->pin_ins[i].identifier);

    wait_for_file(
        configuration->pin_ins[i].direction_path);

    // Set the pin's direction
    write_to_file(
        configuration->pin_ins[i].direction_path,
        "in");

    wait_for_file(
        configuration->pin_ins[i].edge_path);

    // Set the interrupts for the pin
    PinEdge edge = configuration->pin_ins[i].edge;

    write_to_file(
        configuration->pin_ins[i].edge_path,
        edge == RISING ? "rising" :
        edge == FALLING ? "falling" :
        edge == BOTH ? "both" :
        "none");

    wait_for_file(configuration->pin_ins[i].value_path);
  }

  for(i = 0; i < configuration->num_pin_outs; i++) {
    // export the pin
    write_to_file(
        configuration->export_path,
        configuration->pin_outs[i].identifier);

    wait_for_file(
        configuration->pin_outs[i].direction_path);

    // Set the pin's direction
    write_to_file(
        configuration->pin_outs[i].direction_path,
        "out");

    wait_for_file(configuration->pin_outs[i].value_path);
  }

}
