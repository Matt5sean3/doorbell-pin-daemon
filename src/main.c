
#include "pin_daemon.h"
#include <unistd.h>

const PinInConfiguration pin_ins[] = {
  { // light
    "18",
    "/sys/class/gpio/gpio18/direction",
    "/sys/class/gpio/gpio18/value",
    "/sys/class/gpio/gpio18/edge",
    BOTH,
    NULL,
    NULL
  },
  { // button
    "23",
    "/sys/class/gpio/gpio23/direction",
    "/sys/class/gpio/gpio23/value",
    "/sys/class/gpio/gpio23/edge",
    HIGH,
    NULL,
    NULL
  }
};

const PinOutConfiguration pin_outs[] = {
  { // back doorbell
    "24",
    "/sys/class/gpio/gpio24/direction",
    "/sys/class/gpio/gpio24/value"
  },
  { // front doorbell
    "25",
    "/sys/class/gpio/gpio25/direction",
    "/sys/class/gpio/gpio25/value"
  }
};

const PinConfiguration configuration = {
  // export file
  "/sys/class/gpio/export",
  // unexport file
  "/sys/class/gpio/unexport",
  // number of input pins
  2,
  pin_ins,
  // number of output pins
  2,
  pin_outs
};

int main(int argc, char** argv) {

  // export the pins
  pin_start(&configuration);

  // Ring the backdoor bell
  pin_set_state(&configuration.pin_outs[0], HIGH);

  // Wait a second
  sleep(1);

  // Turn the pin off
  pin_set_state(&configuration.pin_outs[0], LOW);

  // unexport the pins
  pin_finish(&configuration);
}

