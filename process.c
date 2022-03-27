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

void set_fg(int pgid) {
  ignore_signal(SIGTTOU);
  ignore_signal(SIGTTIN);
  /* Make foreground */
  if (tcsetpgrp(0, pgid) == -1) {
    perror("tcsetpgrp");
  }
}

void wait_child(int pid) {
  int status;

  waitpid(pid, &status, WUNTRACED | WCONTINUED);
  if (WIFEXITED(status)) {
    set_fg(getpgrp());
    delete_job(pid);
  } else if (WIFSIGNALED(status)) {
    switch (WTERMSIG(status)) {
      case SIGINT:
        puts("detect SIGINT");
        set_fg(getpgrp());
        delete_job(pid);
        break;
    }
  } else if (WIFSTOPPED(status)) {
    switch (WSTOPSIG(status)) {
      case SIGTSTP:
        puts("detect SIGTSTP");
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

void do_fg(char** argv) {
  struct job* fgjob;

  if (argv[1]) {
    int jid = strtol(argv[1], NULL, 10);
    fgjob = find_job_from_jobid(jid);
    if (!fgjob) {
      fprintf(stderr, "Cannot find specified jobid %d.\n", jid);
      return;
    }
  } else {
    /* TODO: save current job */
    return;
  }

  if (killpg(fgjob->pgid, SIGCONT) == -1) {
    perror("killpg");
  }
  set_fg(fgjob->pgid);
  wait_child(fgjob->pid);
  set_fg(getpgrp());
}
