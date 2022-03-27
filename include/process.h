#ifndef PROCESS_H_
#define PROCESS_H_

void wait_child(int pid);

void set_fg(int pgrp);
void do_fg(char** argv);

#endif  // PROCESS_H_
