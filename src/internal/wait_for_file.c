
#include "pin_daemon_internal.h"
#include <stdio.h>

void wait_for_file(const char* path) {
  FILE* f;
  while((f = fopen(path, "w")) == NULL);
  fclose(f);
}

