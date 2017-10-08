
#include <stdio.h>

int write_to_file(const char* path, const char* text) {
  int written;
  FILE* f = fopen(path, "w");
  if(!f)
    return 0;
  written = fprintf(f, text);
  fclose(f);
  return written;
}

