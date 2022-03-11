#include "exec.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define PATH_TOKEN_MAX  32
#define COMMAND_LEN_MAX 64
#define PATH_LEN_MAX    256

static char** get_env() {
  char* env;
  char** envs = malloc(sizeof(char*) * PATH_TOKEN_MAX);
  int index = 0;

  char* env_string = getenv("PATH");

  /* tokenize PATH */
  const char* delimiter = ":";
  env = strtok(env_string, delimiter);
  if (!env) {
    fprintf(stderr, "Cannot find :\n");
    return NULL;
  }
  envs[index++] = env;

  while (env) {
    /* strtok() expects NULL for 1st argument in 2nd run. */
    env = strtok(NULL, delimiter);
    envs[index++] = env;
  }
  envs[index] = NULL;
  return envs;
}

int tursh_exec(char** argv) {
  int pid, status;
  char** envs = malloc(sizeof(char*) * PATH_TOKEN_MAX);
  char** envp = malloc(sizeof(char*) * 2);
  char* command_original = malloc(COMMAND_LEN_MAX);

  if ((pid = fork()) > 0) {
    /* Parent */
    wait(&status);
  } else if (pid == 0) {
    /* Child */
    envs = get_env();
    command_original = argv[0];

    /* envp for execve */
    char* envs_format = malloc(PATH_LEN_MAX + 7); /* PATH="..." */
    sprintf(envs_format, "PATH=\"%s\"", getenv("PATH"));
    envp[0] = envs_format;
    envp[1] = NULL;

    /* Try PATH */
    int index = 0;
    do {
      if (!envs[index]) {
        /* Already searched every path.*/
        fprintf(stderr, "Command not found.\n");
        exit(1);
      }
      char fullpath[PATH_LEN_MAX + COMMAND_LEN_MAX] = {0};
      sprintf(fullpath, "%s/%s", envs[index++], command_original);
      argv[0] = fullpath;
      if (execve(argv[0], argv, envp) == -1) {
        /* Incorrect PATH */
        continue;
      } else {
        /* Correct PATH */
        break;
      }
    } while (1);
    exit(1);
  }
  return 0;
}
