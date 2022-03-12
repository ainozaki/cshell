#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int execute_builtin(char** argv) {
  if (!strncmp(argv[0], "exit", 5)) {
    /* exit command */
    exit(0);
  }
  if (!strncmp(argv[0], "cd", 3)) {
    /* cd command */
    if (!argv[1]) {
      printf("usage: cd <path>\n");
      return 0;
    }
    if (chdir(argv[1]) != 0) {
      perror("cd");
    }
    return 0;
  } else {
    /* Not built-in command */
    return 1;
  }
}
