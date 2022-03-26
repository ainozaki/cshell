#include "job.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static struct job* current;
static struct job* prev;

static int jobid = 1;
static struct job* jobs;

static int find_jobid_from_pgid(int pgid) {
  struct job* entry = jobs;

  do {
    if (!entry) {
      break;
    }
    if (entry->pgid == pgid) {
      return entry->jobid;
    }
    entry = entry->next;
  } while (entry != jobs);

  return -1;
}

static void job_state_to_string(enum job_state state, char* buff) {
  switch (state) {
    case JOB_RUNNING:
      strcpy(buff, "Running");
      break;
    case JOB_STOPPED:
      strcpy(buff, "Stopped");
      break;
  }
}

static void job_command_to_string(char** command, char* buff) {
  for (int i = 0; command[i]; i++) {
    strcpy(buff, command[i]);
    buff += sizeof(command[i]);
  }
}

void init_jobs() {
  jobs = (struct job*)malloc(sizeof(struct job));
  jobs->jobid = -1;
  jobs->next = jobs;
  jobs->prev = jobs;
}

void add_job(int pid, int pgid, char** command) {
  int id;
  struct job* entry = (struct job*)malloc(sizeof(struct job));

  if ((jobid = find_jobid_from_pgid(pgid)) == -1) {
    id = jobid++;
  }

  entry->jobid = id;
  entry->command = command;
  entry->pid = pid;
  entry->pgid = pgid;

  entry->prev = jobs;
  entry->next = jobs->next;
  jobs->next->prev = entry;
  jobs->next = entry;
}

int delete_job(int pid) {
  struct job* entry;
  for (entry = jobs->next; entry != jobs; entry = entry->next) {
    if (entry->pid == pid) {
      entry->prev->next = entry->next;
      entry->next->prev = entry->prev;
      return 0;
    }
  }
  printf("Failed to delete job\n");
  return -1;
}

void show_jobs() {
  struct job* entry;
  char* buff_state = malloc(10);
  char* buff_command = malloc(30);

  for (entry = jobs->next; entry != jobs; entry = entry->next) {
    job_state_to_string(entry->state, buff_state);
    job_command_to_string(entry->command, buff_command);
    printf("[%d] %10s %20s\n", entry->jobid, buff_state, buff_command);
    entry = entry->next;
  }

  free(buff_state);
  free(buff_command);
}

void set_fg(int pgid) {
  /* Make foreground */
  if (tcsetpgrp(STDOUT_FILENO, pgid) != 0) {
    perror("tcsetpgrp");
    exit(1);
  }
  puts("sigtstp");
}
