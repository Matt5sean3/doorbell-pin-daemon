
#ifndef PIN_DAEMON_H_

#define PIN_DAEMON_H_

#define NUM_GPIO_PINS 27
#define NULL 0
#include <time.h>

typedef struct PinInConfiguration PinInConfiguration;
typedef struct PinOutConfiguration PinOutConfiguration;

typedef enum PinState {
  LOW,
  HIGH
} PinState;

typedef enum PinEdge {
  NONE,
  RISING,
  FALLING,
  BOTH
} PinEdge;

typedef void (*Reaction)(
    PinState state,
    void* user_pointer);

struct PinInConfiguration {
  const char* identifier;
  const char* direction_path;
  const char* value_path;
  const char* edge_path;

  PinEdge edge;
  Reaction reaction;
  void* reaction_user_pointer;

  struct timespec debounce;
  int ignore_ephemeral_changes;
};

struct PinOutConfiguration {
  const char* identifier;
  const char* direction_path;
  const char* value_path;
};

typedef struct PinConfiguration {

  // Number of milliseconds to poll before the daemon quits
  long daemon_sleep_ms;

  const char* export_path;
  const char* unexport_path;

  int num_pin_ins;
  const PinInConfiguration* pin_ins;

  int num_pin_outs;
  const PinOutConfiguration* pin_outs;

} PinConfiguration;

// Run the declaratively defined pin context
int pin_run(
    const PinConfiguration* configuration);

int pin_start(
    const PinConfiguration* configuration);

int pin_finish(
    const PinConfiguration* configuration);

PinState pin_get_state(
    const PinInConfiguration* pin_in);

int pin_set_state(
    const PinOutConfiguration* pin_out,
    PinState state);

int pin_pulse(
    const PinOutConfiguration* pin_out,
    int duration_ms);

#endif

