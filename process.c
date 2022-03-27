#include "process.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "job.h"

void wait_child(int pid) {
  int status;

  waitpid(pid, &status, WUNTRACED | WCONTINUED);
  if (WIFEXITED(status)) {
    set_fg(getpgrp());
    delete_job(pid);
  } else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) {
    puts("detect SIGINT");
    set_fg(getpgrp());
    delete_job(pid);
  } else if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTSTP) {
    puts("detect SIGTSTP");
    set_fg(getpgrp());
    stop_job(pid);
  } else if (WIFSIGNALED(status)) {
    printf(
        "[%d]: child (%d) terminates or suspended unexpectedly [signal: "
        "%d]\n",
        getpid(), pid, WTERMSIG(status));
  } else {
    printf("Failed to exec some readon\n");
  }
}
