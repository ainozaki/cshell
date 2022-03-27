#include "exec.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "argv.h"
#include "builtin.h"
#include "input.h"
#include "job.h"
#include "process.h"
#include "redirect.h"
#include "signal_handle.h"
#include "utils.h"

static void extract_next_command(char** exec, char** argv, int pipe_pos) {
  assert(pipe_pos != 0);
  int argc = get_argc(argv);

  if (pipe_pos > 0) {
    /* pipe exists */
    extract_argv(exec, argv, /*pos=*/0, /*size=*/pipe_pos);
    delete_argv(argv, /*pos=*/0, /*size=*/pipe_pos + 1);
  } else if (pipe_pos == -1) {
    /* No pipe */
    extract_argv(exec, argv, 0, argc);
    delete_argv(argv, 0, argc);
  }
}

/* Run execve() resolving PATH */
static void tursh_execve(char** exec) {
  char** envs = malloc(sizeof(char*) * PATH_TOKEN_MAX);
  char** envp = malloc(sizeof(char*) * 2);
  char* command_original = malloc(COMMAND_LEN_MAX);

  /* env for execve() 1st arg */
  envs = get_env_list();
  command_original = exec[0];

  /* envp for execve() 3rd arg */
  envp[0] = get_envp_path();
  envp[1] = NULL;

  /* Handle redirect */
  do_redirect(exec);

  /* Try all PATH */
  int index = 0;
  do {
    if (!envs[index]) {
      /* Already searched every path.*/
      fprintf(stderr, "Command not found.\n");
      exit(1);
    }
    /* Create full path */
    char fullpath[PATH_LEN_MAX + COMMAND_LEN_MAX] = {0};
    sprintf(fullpath, "%s/%s", envs[index++], command_original);
    exec[0] = fullpath;
    if (execve(exec[0], exec, envp) == -1) {
      /* Incorrect PATH */
      continue;
    } else {
      /* Correct PATH */
      break;
    }
  } while (1);
  free(envs);
  free(envp);
  free(command_original);
}

int tursh_exec(char** argv) {
  int pid;
  char** exec = malloc(sizeof(char*) * COMMAND_LEN_MAX);

  bool pipe_exists = false; /* for next command */
  int pipefds[] = {0, 1};
  int prev_pipefds[] = {0, 1};
  int in_fd, out_fd;
  int pgid = -1;

  while (argv[0]) {
    /* Pipe */
    /* Update pipe */
    if (pipe_exists) {
      prev_pipefds[0] = pipefds[0];
      prev_pipefds[1] = pipefds[1];
    }

    int pipe_pos = search_argv(argv, "|");
    if (pipe_pos > 0) { /* pipe exists */
      pipe_exists = true;
      pipe(pipefds);
    } else if (pipe_pos == -1) { /* No pipe */
      pipe_exists = false;
      pipefds[0] = 0;
      pipefds[1] = 1;
    } else if (pipe_pos == 0) {
      fprintf(stderr, "Unexpected | location.\n");
      return -1;
    }
    in_fd = prev_pipefds[0];
    out_fd = pipefds[1];

    /* Determine next command */
    extract_next_command(exec, argv, pipe_pos);
    if (!exec) {
      return -1;
    }

    /* fork */
    if ((pid = fork()) > 0) {
      /* Parent */
      /* Close fd */
      if (in_fd != 0) {
        close(in_fd);
      }
      if (out_fd != 1) {
        close(out_fd);
      }

      /* Set child's pgid */
      if (pgid == -1) {
        pgid = pid;
        setpgid(pid, pid);
      } else {
        setpgid(pid, pgid);
      }

      add_job(pid, pgid, exec);

      /* Wait for child */
      set_fg(pgid);

      wait_child(pid);

      set_fg(getpgrp());

    } else if (pid == 0) {
      /* Child */
      /* Update fds */
      if (in_fd != 0) {
        if (dup2(in_fd, 0) == -1) {
          perror("dup2");
          exit(1);
        }
        close(in_fd);
        close(prev_pipefds[1]);
      }

      if (out_fd != 1) {
        if (dup2(out_fd, 1) == -1) {
          perror("dup2");
          exit(1);
        }
        close(out_fd);
        close(pipefds[0]);
      }

      /* Set own pgid */
      if (pgid == -1) {
        setpgid(pid, pid);
      } else {
        setpgid(pid, pgid);
      }

      /* Set signal default */
      default_signal(SIGINT);
      default_signal(SIGTSTP);
      default_signal(SIGTTOU);

      /* Exec commands */
      tursh_execve(exec);
      exit(0);
    }
  }
  free(exec);
  return 0;
}
