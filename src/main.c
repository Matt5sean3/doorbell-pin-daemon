
#include "pin_daemon.h"
#include <time.h>
#include <stdio.h>

static void ring_doorbell(PinState state, void* userptr);
static void light_change(PinState state, void* userptr);

#define LIGHT_LOG "/mnt/HackRVA-NAS/lights.log"

const PinInConfiguration pin_ins[] = {
  { // light
    "4",
    "/sys/class/gpio/gpio4/direction",
    "/sys/class/gpio/gpio4/value",
    "/sys/class/gpio/gpio4/edge",
    PIN_EDGE_BOTH,

    light_change, // callback
    NULL, // callback userptr
    {0, 0}, // no debounce
    1 // ignore ephemeral changes
  },
  { // button
    "17",
    "/sys/class/gpio/gpio17/direction",
    "/sys/class/gpio/gpio17/value",
    "/sys/class/gpio/gpio17/edge",
    PIN_EDGE_BOTH,

    ring_doorbell, // callback
    NULL, // callback userptr
    {0, 200000000}, // 20ms debounce
    1 // ignore ephemeral changes
  }
};

const PinOutConfiguration pin_outs[] = {
  { // back doorbell
    "18",
    "/sys/class/gpio/gpio18/direction",
    "/sys/class/gpio/gpio18/value"
  },
  { // front doorbell
    "27",
    "/sys/class/gpio/gpio27/direction",
    "/sys/class/gpio/gpio27/value"
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

static void ring_doorbell(PinState state, void* userptr) {
  if(state != PIN_STATE_HIGH)
    return;
  pin_set_state(&pin_outs[1], PIN_STATE_HIGH);
  nanosleep(&ring_hold, NULL);
  pin_set_state(&pin_outs[1], PIN_STATE_LOW);
}

static void light_change(PinState state, void* userptr) {
  FILE* f = fopen(LIGHT_LOG, "a");
  if(f == NULL)
    printf("Failed to open LIGHT_LOG file: %s\n", LIGHT_LOG);
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);
  fprintf(f, "%lld:%9ld %s\n", (long long) current_time.tv_sec, current_time.tv_nsec, state == PIN_STATE_HIGH ? "B" : "D");
  fclose(f);
}

