
#ifndef PIN_DAEMON_TEST_H_
#define PIN_DAEMON_TEST_H_

#include <stdio.h>
#include <string.h>

typedef int (*Test)(int argc, char** argv);

typedef struct TestMapping {
  char* name;
  Test test;
} TestMapping;

int test_socket_triggers_callback_on_read(int argc, char** argv);

#endif

