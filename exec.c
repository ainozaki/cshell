#include "exec.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int tursh_exec() {
  int pid;
  int status;
  char *argv[] = {"/usr/bin/ls", "-l", NULL};

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
