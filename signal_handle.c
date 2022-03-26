#include "signal_handle.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void sigtstp_handler(int signal) {
  /* Make foreground */
  // if (tcsetpgrp(STDOUT_FILENO, getpgrp()) != 0) {
  //  perror("tcsetpgrp");
  //  exit(1);
  //}
  puts("sigtstp");
}

void ignore_signal(int signal) {
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  act.sa_handler = SIG_IGN;
  sigaction(signal, &act, NULL);
}

void default_signal(int signal) {
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  act.sa_handler = SIG_DFL;
  sigaction(signal, &act, NULL);
}

void set_sigtstp_handler() {
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  act.sa_handler = sigtstp_handler;
  sigaction(SIGTSTP, &act, NULL);
}
