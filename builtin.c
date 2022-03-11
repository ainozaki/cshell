#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int execute_builtin(char** argv) {
  return 1;
}

void execute_builtin_exit(char** argv) {
  if (!strncmp(argv[0], "exit", 5)) {
    exit(0);
  }
}
