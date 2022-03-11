#include "signal_handle.h"

#include <signal.h>
#include <string.h>

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
