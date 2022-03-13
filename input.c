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
  argv[index] = NULL;
  return argv;
}

int get_argc(char* argv[]) {
  int n = 0;
  while (argv[n]) {
    n++;
    continue;
  }
  return n;
}

int search_argv(char* argv[], char* arg) {
  for (int i = 0; i < get_argc(argv); i++) {
    if (strcmp(argv[i], arg) == 0) {
      return i;
    }
  }
  return -1;
}

void delete_argv(char* argv[], int pos, int n) {
  int argc = get_argc(argv);
  for (int i = pos + n; i < argc; i++) {
    argv[i - n] = argv[i];
  }
  argv[argc - n] = NULL;
}

void extract_argv(char* dst[], char* src[], int pos, int n) {
  for (int i = 0; i < n; i++) {
    dst[i] = src[pos + i];
  }
  dst[n] = NULL;
}

void dump_argv(char* name, char** argv) {
  printf("---------------\n");
  printf("%s\n", name);
  for (int i = 0; i < get_argc(argv); i++) {
    printf("%s[%d] =  %s\n", name, i, argv[i]);
  }
  printf("---------------\n");
}
