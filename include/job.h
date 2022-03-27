#ifndef JOB_H_
#define JOB_H_

enum job_state { JOB_RUNNING, JOB_STOPPED };

struct job {
  int jobid;
  int pid;
  int pgid;
  enum job_state state;
  char* command;
  struct job* next;
  struct job* prev;
};

void add_job(int pid, int pgid, char** command);
int delete_job(int pid);
int stop_job(int pid);

void show_jobs();
void init_jobs();

extern struct job* jobs;

void set_fg(int pgrp);
int do_fg(char** argv);

#endif  // JOB_H_
