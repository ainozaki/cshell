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

void delete_argv(char* argv[], int pos, int size) {
  int argc = get_argc(argv);
  for (int i = pos + size; i < argc; i++) {
    argv[i - size] = argv[i];
  }
  argv[argc - size] = NULL;
}

void extract_argv(char* dst[], char* src[], int pos, int size) {
  for (int i = 0; i < size; i++) {
    dst[i] = src[pos + i];
  }
  dst[size] = NULL;
}

void dump_argv(char* name, char** argv) {
  printf("---------------\n");
  printf("%s\n", name);
  for (int i = 0; i < get_argc(argv); i++) {
    printf("%s[%d] =  %s\n", name, i, argv[i]);
  }
  printf("---------------\n");
}
