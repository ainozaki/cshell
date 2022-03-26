#ifndef SIGNAL_HANDLE_H_
#define SIGNAL_HANDLE_H_

void ignore_signal(int signal);
void default_signal(int signal);

void set_sigtstp_handler();

#endif
