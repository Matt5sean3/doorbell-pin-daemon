
#include "pin_daemon.h"
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <stdlib.h>

static void ring_doorbell(PinState state, void* userptr);
static void light_change(PinState state, void* userptr);
static void rfid_reaction(int stream, void* userptr);
static void fifo_reaction(int stream, void* userptr);

#define LIGHT_LOG "/mnt/HackRVA-NAS/lights.log"
#define PIN_OUT(pin) { \
    pin, \
    "/sys/class/gpio/gpio" pin "/direction", \
    "/sys/class/gpio/gpio" pin "/value" \
  }

#define PIN_IN(pin, edges, callback, userptr, debounce_s, debounce_ns, ignore_ephemeral) { \
    pin, \
    "/sys/class/gpio/gpio" pin "/direction", \
    "/sys/class/gpio/gpio" pin "/value", \
    "/sys/class/gpio/gpio" pin "/edge", \
    edges, \
    callback, \
    userptr, \
    { debounce_s, debounce_ns }, \
    ignore_ephemeral \
  }

const PinInConfiguration pin_ins[] = {
  // light sensor
  PIN_IN("4", PIN_EDGE_BOTH, light_change, NULL, 0, 0, 1),
  // button
  // 20ms debounce
  PIN_IN("17", PIN_EDGE_BOTH, ring_doorbell, NULL, 0, 200000000, 1)
};

const PinOutConfiguration pin_outs[] = {
  PIN_OUT("18"), // back doorbell
  PIN_OUT("27"), // front doorbell
  PIN_OUT("22"), // unused 12V control
  PIN_OUT("23"), // strike lock
  PIN_OUT("24") // RFID enable
};

const SocketConfiguration sockets[] = {
  { "/dev/serial0", O_RDONLY | O_NONBLOCK, rfid_reaction, NULL, POLLIN, BAUD2400 },
  // Create a fifo that can be manipulated
  { "pin-daemon", O_RDONLY | O_NONBLOCK, fifo_reaction, NULL, POLLRDNORM, FIFO }
};

const PinConfiguration configuration = {
  86400000, /* an hour in milliseconds 1000 * 60 * 60 * 24 */
  // export file
  "/sys/class/gpio/export",
  // unexport file
  "/sys/class/gpio/unexport",
  // input pins
  2,
  pin_ins,
  // output pins
  5,
  pin_outs,
  // sockets
  2,
  sockets
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

static void rfid_reaction(int stream, void* userptr) {
  // Hopefully it doesn't go out of sync
  ssize_t nread;
  char buffer[21];
  long num = 0;
  while((nread = read(stream, buffer, 21)) > 0) {
    buffer[nread] = 0;
    buffer[20] = 0;
    // Convert second byte and onward from hex into a long
    num = strtol(&buffer[2], NULL, 16);
    // ignore first byte
    printf("Read from RFID: %li\n", buffer, num);
    // Needs to check with
  }
}

static void fifo_reaction(int stream, void* userptr) {
  ssize_t nread;
  char buffer[21];
  while((nread = read(stream, buffer, 20)) > 0) {
    buffer[nread] = 0;
    printf("Read %i from FIFO: %s\n", nread, buffer);
  }
  if(nread < 0)
    printf("Should Quit\n");
}

