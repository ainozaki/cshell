#include "exec.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int tursh_exec(char **argv) {
  int pid;
  int status;

  if ((pid = fork()) > 0) {
    /* Parent */
    wait(&status);
  } else if (pid == 0) {
    /* Child */
    if (execve(argv[0], argv, &argv[2]) == -1) {
      perror("execve");
    }
    exit(1);
  }
  return 0;
}
