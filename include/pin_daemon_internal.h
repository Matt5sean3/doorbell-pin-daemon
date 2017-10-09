
#ifndef PIN_DAEMON_INTERNAL_H_
#define PIN_DAEMON_INTERNAL_H_

#include "pin_daemon.h"

#include <stdio.h>

int write_to_file(const char* path, const char* text);
void wait_for_file(const char* path);
char drain_sysfs_file(FILE* f);

#endif

