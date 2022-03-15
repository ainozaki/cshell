#include "argv.h"

#include <stdio.h>
#include <string.h>

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
