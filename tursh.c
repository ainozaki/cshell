#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "builtin.h"
#include "exec.h"
#include "input.h"

#define CWD_SIZE_MAX 64

int main() {
  do {
    char* buf = malloc(CWD_SIZE_MAX);
    printf("\x1b[36m%s\x1b[39m$ ", getcwd(buf, CWD_SIZE_MAX));

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
