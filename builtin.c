#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CWD_SIZE_MAX 64

#include "input.h"

static int do_cd(char** argv) {
  if (!argv[1]) {
    printf("usage: cd <path>\n");
    return 0;
  }
  if (chdir(argv[1]) != 0) {
    perror("cd");
    return 0;
  }
  delete_argv(argv, 0, 2);
  return 0;
}

static int do_pwd(char** argv) {
  char* buf = malloc(CWD_SIZE_MAX);
  printf("%s\n", getcwd(buf, CWD_SIZE_MAX));
  free(buf);
  delete_argv(argv, 0, 1);
  return 0;
}

int if_exit(char** argv) {
  if (!strncmp(argv[0], "exit", 5)) {
    return 0;
  }
  return 1;
}

int execute_builtin(char** argv) {
  if (!strncmp(argv[0], "cd", 3)) {
    return do_cd(argv);
  } else if (!strncmp(argv[0], "pwd", 4)) {
    return do_pwd(argv);
  } else {
    /* Not built-in command */
    return 1;
  }
}
