#include "process.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "argv.h"
#include "job.h"
#include "signal_handle.h"

void wait_child(int pid) {
  int status;

  waitpid(pid, &status, WUNTRACED);
  if (WIFEXITED(status)) {
    set_fg(getpgrp());
    delete_job(pid);
  } else if (WIFSIGNALED(status)) {
    switch (WTERMSIG(status)) {
      case SIGINT:
        puts("^C");
        set_fg(getpgrp());
        delete_job(pid);
        break;
    }
  } else if (WIFSTOPPED(status)) {
    switch (WSTOPSIG(status)) {
      case SIGTSTP:
        puts("^Z");
        set_fg(getpgrp());
        stop_job(pid);
        break;
    }
  } else if (WIFSIGNALED(status)) {
    printf(
        "[%d]: child (%d) terminates or suspended unexpectedly [signal: "
        "%d]\n",
        getpid(), pid, WTERMSIG(status));
  } else {
    printf("Failed to exec some reason\n");
  }
}
