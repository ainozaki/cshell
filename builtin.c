#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CWD_SIZE_MAX 64

#include "argv.h"
#include "input.h"
#include "job.h"
#include "process.h"

static void do_cd(char** argv) {
  if (!argv[1]) {
    printf("usage: cd <path>\n");
    return;
  }
  if (chdir(argv[1]) != 0) {
    perror("cd");
    return;
  }
}

static void do_pwd(char** argv) {
  char* buf = malloc(CWD_SIZE_MAX);
  printf("%s\n", getcwd(buf, CWD_SIZE_MAX));
  free(buf);
}

int execute_builtin(char** argv) {
  if (!strncmp(argv[0], "cd", 3)) {
    do_cd(argv);
    return 0;
  } else if (!strncmp(argv[0], "pwd", 4)) {
    do_pwd(argv);
    return 0;
  } else if (!strncmp(argv[0], "jobs", 5)) {
    show_jobs();
    return 0;
  } else if (!strncmp(argv[0], "fg", 3)) {
    do_fg(argv);
    return 0;
  } else if (!strncmp(argv[0], "bg", 3)) {
    do_bg(argv);
    return 0;
  } else if (!strncmp(argv[0], "exit", 5)) {
    exit(0);
  } else {
    /* Not built-in command */
    return 1;
  }
}
