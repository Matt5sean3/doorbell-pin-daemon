
#include "pin_daemon.h"
#include <unistd.h>

void ring_doorbell(PinState state, void* userptr);

const PinInConfiguration pin_ins[] = {
  { // light
    "18",
    "/sys/class/gpio/gpio18/direction",
    "/sys/class/gpio/gpio18/value",
    "/sys/class/gpio/gpio18/edge",
    BOTH,

    NULL, // callback
    NULL, // callback userptr
    {0, 0}, // no debounce
    1 // ignore ephemeral changes
  },
  { // button
    "23",
    "/sys/class/gpio/gpio23/direction",
    "/sys/class/gpio/gpio23/value",
    "/sys/class/gpio/gpio23/edge",
    BOTH,

    ring_doorbell, // callback
    NULL, // callback userptr
    {0, 200000000}, // 20ms debounce
    1 // ignore ephemeral changes
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
  86400000, /* an hour in milliseconds 1000 * 60 * 60 * 24 */
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

  pin_run(&configuration);

}

// Hold the ring for 200ms
const struct timespec ring_hold = {
  0,
  200000000
};

void ring_doorbell(PinState state, void* userptr) {
  if(state != HIGH)
    return;
  pin_set_state(&pin_outs[1], HIGH);
  nanosleep(&ring_hold, NULL);
  pin_set_state(&pin_outs[1], LOW);
}

