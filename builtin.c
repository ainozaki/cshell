#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int execute_builtin(char** argv) {
  if (!strncmp(argv[0], "exit", 5)) {
    exit(0);
  }
  if (!strncmp(argv[0], "cd", 3)) {
    /* TODO */
    return 0;
  } else {
    /* Not built-in command */
    return 1;
  }
}
