#include "input.h"

#include <errno.h>
#include <input.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TURSH_TOKEN_MAX 32

char** tursh_input() {
  char *line, *token;
  size_t readlen, len = 0;
  int index = 0;
  char** argv = malloc(sizeof(char*) * TURSH_TOKEN_MAX);

  /* get stdin */
  readlen = getline(&line, &len, stdin);
  if (readlen == -1) {
    return NULL;
  }

  /* parse */
  const char* delimiter = " \n";
  token = strtok(line, delimiter);
  if (!token) {
    return NULL;
  }
  argv[index++] = token;
  while (token) {
    /* strtok() expects NULL for 1st argument in 2nd run. */
    token = strtok(NULL, delimiter);
    argv[index++] = token;
  }
  /* Add NULL at the end of argv list. */
  argv[index] = NULL;
  return argv;
}
