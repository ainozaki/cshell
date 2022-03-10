#include <errno.h>
#include <input.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TURSH_TOKEN_MAX 32

char **tursh_input() {
  char *line, *token;
  size_t readlen, len = 0;
  int index = 0;
  char **argv = malloc(sizeof(char *) * TURSH_TOKEN_MAX);

  /* get stdin */
  readlen = getline(&line, &len, stdin);

  /* parse */
  const char *delimiter = " \n";
  token = strtok(line, delimiter);
  if (!token) {
    fprintf(stderr, "Cannot find any whitespace or nl.\n");
    return NULL;
  }
  argv[index++] = token;
  while (token) {
    /* strtok() expects NULL for 1st argument in 2nd run. */
    token = strtok(NULL, delimiter);
    argv[index++] = token;
  }
  return argv;
}
