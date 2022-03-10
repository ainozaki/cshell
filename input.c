#include <errno.h>
#include <input.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define TURSH_ARGV_SIZE 3

char **tursh_input() {
  char *line;
  char **argv;
  size_t readlen, len = 0;

  /* get stdin */
  readlen = getline(&line, &len, stdin);

  /* parse */
  argv = malloc(sizeof(char *) * TURSH_ARGV_SIZE);
  argv[0] = "/usr/bin/ls";
  argv[1] = "-l";
  argv[2] = NULL;
  return argv;
}
