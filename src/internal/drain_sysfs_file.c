
#include "pin_daemon_internal.h"

#define MAX_BUF 64

char drain_sysfs_file(FILE* f) {
  char received_data[MAX_BUF];
  fseek(f, 0L, SEEK_SET);
  fread(received_data, sizeof(char), MAX_BUF, f);
  return received_data[0];
}

