#include "utils.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_envp_path() {
  char* envp_path = malloc(PATH_LEN_MAX + 7); /* PATH="..." */
  sprintf(envp_path, "PATH=\"%s\"", getenv("PATH"));
  return envp_path;
}

char** get_env_list() {
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
