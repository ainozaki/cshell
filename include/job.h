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
extern struct job* jobs;

void init_jobs();
void add_job(int pid, int pgid, char** command);
int delete_job(int pid);
int stop_job(int pid);

void show_jobs();
struct job* find_job_from_jobid(int jobid);

void set_fg(int pgid);
void do_fg(char** argv);
void do_bg(char** argv);

#endif  // JOB_H_
