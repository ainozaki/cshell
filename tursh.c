#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "argv.h"
#include "builtin.h"
#include "exec.h"
#include "input.h"
#include "job.h"
#include "signal_handle.h"

#define CWD_SIZE_MAX 64

static void show_prompt(char* buf) {
  /* Show current working directory */
  printf("\x1b[36m%s\x1b[39m$ ", getcwd(buf, CWD_SIZE_MAX));
}

int main() {
  char* buf = malloc(CWD_SIZE_MAX);
  init_jobs();

  do {
    show_prompt(buf);

    /* Ignore SIGINT & SIGTTOU */
    ignore_signal(SIGINT);
    ignore_signal(SIGTSTP);
    ignore_signal(SIGTTIN);
    ignore_signal(SIGTTOU);

    /* input */
    char** argv = tursh_input();
    if (!argv) {
      continue;
    }

    /* exec built-in command */
    if (execute_builtin(argv) == 0) {
      free(argv);
      continue;
    }

    /* exec */
    tursh_exec(argv);
    free(argv);

  } while (1);

  free(buf);
  return 0;
}
