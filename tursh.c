#include <errno.h>
#include <stddef.h>
#include <stdio.h>

#include "exec.h"
#include "input.h"

int main() {
  do {
    printf("tursh > ");

    /* input */
    char **argv = tursh_input();

    /* build-in */

    /* exec */
    if (tursh_exec(argv) != 0) {
      continue;
    }
  } while (1);

  return 0;
}
