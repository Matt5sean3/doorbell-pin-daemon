cmake_minimum_required(VERSION 3.0)
project(pin_daemon)

include_directories(include)

add_executable(pin_daemon
  src/internal/write_to_file.c
  src/internal/wait_for_file.c
  src/internal/drain_sysfs_file.c
  src/pin_start.c
  src/pin_finish.c
  src/pin_get_state.c
  src/pin_set_state.c
  src/pin_run.c
  src/main.c)

