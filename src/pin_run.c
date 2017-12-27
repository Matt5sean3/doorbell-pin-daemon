
#include "pin_daemon_internal.h"
#include <stdio.h>
#include <poll.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// Node types for linked lists
typedef struct InputPinNode InputPinNode;
typedef struct OutputPinNode OutputPinNode;

#define MAX_BUF 64

static int time_difference_exceeds(
    struct timespec duration,
    struct timespec earlier,
    struct timespec later) {

  struct timespec diff;
  diff.tv_sec = later.tv_sec - earlier.tv_sec;
  diff.tv_nsec = later.tv_nsec - earlier.tv_nsec;

  if(diff.tv_nsec < 0) {
    diff.tv_sec -= 1;
    diff.tv_nsec += 1000000000;
  }

  return diff.tv_sec > duration.tv_sec ||
    (diff.tv_sec == duration.tv_sec && diff.tv_nsec > duration.tv_nsec);
}

int pin_run(const PinConfiguration* configuration) {
  // TODO handle SIGINT to exit cleanly

  int i;
  int ready;

  // Create the set of polled pins
  struct pollfd poll_fds[configuration->num_sockets + configuration->num_pin_ins];
  struct timespec last_interrupt[configuration->num_pin_ins];
  PinState last_state[configuration->num_pin_ins];
  // streams need to be non-blocking
  int streams[configuration->num_sockets];
  FILE* pin_in_file_handles[configuration->num_pin_ins];

  pin_start(configuration);

  // Open streams
  for(i = 0; i < configuration->num_sockets; i++) {
    int result;
    if(configuration->sockets[i].config == FIFO) {
      result = mkfifo(configuration->sockets[i].file, 0660);
      if(result) {
        printf("Creating FIFO failed\n%s\n", strerror(errno));
      }
    }
    streams[i] = open(configuration->sockets[i].file, configuration->sockets[i].mode);
    if(streams[i] < 0) {
      printf("Opening file failed\n%s\n", strerror(errno));
    } else {
      poll_fds[i].fd = streams[i];
      poll_fds[i].events = configuration->sockets[i].poll_mode;
    }
  }

  for(i = 0; i < configuration->num_pin_ins; i++) {
    int j = i + configuration->num_sockets;
    pin_in_file_handles[i] = fopen(configuration->pin_ins[i].value_path, "r");
    poll_fds[j].fd = fileno(pin_in_file_handles[i]);
    poll_fds[j].events = POLLPRI;
    clock_gettime(CLOCK_MONOTONIC, &last_interrupt[i]);
  }

  do {
    
    ready = poll(poll_fds, configuration->num_pin_ins + configuration->num_sockets, configuration->daemon_sleep_ms);

    struct timespec current_time;
    if(clock_gettime(CLOCK_MONOTONIC, &current_time)) {
      printf("Clock failure!\n");
      break;
    }

    for(i = 0; i < configuration->num_sockets; i++) {
      if(poll_fds[i].revents) {
        configuration->sockets[i].reaction(streams[i], configuration->sockets[i].reaction_user_pointer);
      }
    }

    for(i = 0; i < configuration->num_pin_ins; i++) {
      int j = i + configuration->num_sockets;
      if(poll_fds[j].revents & POLLPRI) {
        FILE* file_handle = pin_in_file_handles[i];
        char status_character = drain_sysfs_file(file_handle);
        PinState state = status_character == '0' ? PIN_STATE_LOW : PIN_STATE_HIGH;

        if(
            (state != last_state[i] || configuration->pin_ins[i].edge != PIN_EDGE_BOTH) &&
            time_difference_exceeds(configuration->pin_ins[i].debounce, last_interrupt[i], current_time)) {

          last_interrupt[i] = current_time;

          if(configuration->pin_ins[i].reaction)
            configuration->pin_ins[i].reaction(
                state,
                configuration->pin_ins[i].reaction_user_pointer);
        }

        last_state[i] = state;
      }
    }
  } while(ready > 0);

  for(i = 0; i < configuration->num_pin_ins; i++) {
    fclose(pin_in_file_handles[i]);
  }

  for(i = 0; i < configuration->num_sockets; i++) {
    close(streams[i]);
  }

  pin_finish(configuration);

}

