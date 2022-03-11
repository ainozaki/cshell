#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "builtin.h"
#include "exec.h"
#include "input.h"

int main() {
  do {
    printf("tursh > ");

    /* input */
    char** argv = tursh_input();
    if (!argv) {
      fprintf(stderr, "Cannot parse.\n");
      continue;
    }

    /* built-in */
    if (execute_builtin(argv) == 0) {
      continue;
    }

    /* exec */
    if (tursh_exec(argv) != 0) {
      continue;
    }
  } while (1);

  return 0;
}
