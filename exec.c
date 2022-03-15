#include "exec.h"

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
#include "input.h"
#include "redirect.h"
#include "signal_handle.h"
#include "utils.h"

int tursh_exec(char** argv) {
  int pid, status;
  char** envs = malloc(sizeof(char*) * PATH_TOKEN_MAX);
  char** envp = malloc(sizeof(char*) * 2);
  char** exec = malloc(sizeof(char*) * COMMAND_LEN_MAX);
  char* command_original = malloc(COMMAND_LEN_MAX);

  bool pipe_exists = false;
  int pipefds[] = {-1, -1};
  int in_fd = 0;
  int out_fd = 1;
  int in_fd_op, out_fd_op;

  while (argv[0]) {
    /* Pipe handling */
    in_fd = 0;
    in_fd_op = -1;
    out_fd = 1;
    out_fd_op = -1;

    if (pipe_exists) {
      in_fd = pipefds[0];
      in_fd_op = pipefds[1];
    }

    int pipe_pos = search_argv(argv, "|");
    int argc = get_argc(argv);

    if (pipe_pos > 0) {
      /* pipe exists */
      pipe_exists = true;
      pipe(pipefds);
      out_fd = pipefds[1];
      out_fd_op = pipefds[0];
      extract_argv(exec, argv, 0, pipe_pos);
      delete_argv(argv, 0, pipe_pos + 1);
    } else if (pipe_pos == -1) {
      /* No pipe */
      pipe_exists = false;
      extract_argv(exec, argv, 0, argc);
      delete_argv(argv, 0, argc);
    } else if (pipe_pos == 0) {
      fprintf(stderr, "Unexpected | location.\n");
      return 1;
    }

    /* fork */
    if ((pid = fork()) > 0) {
      /* Parent */
      if (in_fd != 0) {
        close(in_fd);
      }
      if (out_fd != 1) {
        close(out_fd);
      }

      /* Set child's pgid */
      if (setpgid(pid, pid) != 0) {
        perror("setpgid");
        exit(1);
      }

      /* Ignore SIGINT & SIGTTOU */
      ignore_signal(SIGINT);
      ignore_signal(SIGTTOU);

      /* Make backgrand */
      if (tcsetpgrp(STDOUT_FILENO, pid) != 0) {
        perror("tcsetpgrp");
        exit(1);
      }

      /* Wait for child */
      wait(&status);
      if (!WIFEXITED(status)) {
        printf("[%d]: child (%d) terminates or suspended unexpectedly\n",
               getpid(), pid);
        exit(1);
      }

      /* Make foreground */
      if (tcsetpgrp(STDOUT_FILENO, getpgrp()) != 0) {
        perror("tcsetpgrp");
        exit(1);
      }
    } else if (pid == 0) {
      /* Child */

      if (in_fd != 0) {
        if (dup2(in_fd, 0) == -1) {
          perror("dup2");
          exit(1);
        }
        close(in_fd);
        close(in_fd_op);
      }

      if (out_fd != 1) {
        if (dup2(out_fd, 1) == -1) {
          perror("dup2");
          exit(1);
        }
        close(out_fd);
        close(out_fd_op);
      }

      /* Set own pgid */
      if (setpgid(getpid(), 0) != 0) {
        perror("setpgid");
        exit(1);
      }

      /* Set signal default */
      default_signal(SIGINT);
      default_signal(SIGTTOU);

      /* envp for execve() 3rd arg */
      envp[0] = get_envp_path();
      envp[1] = NULL;

      /* env for execve() 1st arg */
      envs = get_env_list();
      command_original = exec[0];

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
      free(exec);
      free(command_original);
      exit(0);
    }
  }
  return 0;
}
