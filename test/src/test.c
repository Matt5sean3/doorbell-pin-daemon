
#include "test.h"

const TestMapping tests[] = {
  { "test_socket_triggers_callback_on_read", test_socket_triggers_callback_on_read }
};

int num_tests = sizeof(tests);

int main(int argc, char** argv) {
  char* test_name;
  int i;

  if(argc < 2) {
    printf("No test name argument provided");
    return 1;
  }
  test_name = argv[1];
  for(i = 0; i < num_tests; i++) {
    if(strcmp(tests[i].name, test_name) == 0)
      return tests[i].test(argc - 2, &argv[2]);
  }

  printf("Couldn't find the named test");
  return 1;
}

