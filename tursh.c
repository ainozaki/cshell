#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "builtin.h"
#include "exec.h"
#include "input.h"
#include "signal_handle.h"

#define CWD_SIZE_MAX 64

int main() {
  char* buf = malloc(CWD_SIZE_MAX);
  do {
    /* Show current working directory */
    printf("\x1b[36m%s\x1b[39m$ ", getcwd(buf, CWD_SIZE_MAX));

    /* Ignore SIGINT & SIGTTOU */
    ignore_signal(SIGINT);
    ignore_signal(SIGTTOU);

    /* input */
    char** argv = tursh_input();
    if (!argv) {
      fprintf(stderr, "Cannot parse.\n");
      continue;
    }

    /* exec */
    if (tursh_exec(argv) != 0) {
      /* Error */
      continue;
    }
  } while (1);

  free(buf);
  return 0;
}
