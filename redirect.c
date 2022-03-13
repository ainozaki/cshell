#include "redirect.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "input.h"

void do_redirect(char* argv[]) {
  int n = 0, fd;
  n = search_argv(argv, "<");
  if (n >= 0) {
    /* file only */
    fd = open(argv[n + 1], O_RDONLY);
    if (fd < 0) {
      perror(argv[n + 1]);
      exit(1);
    }
    close(0);
    dup2(fd, 0);
    close(fd);

    delete_argv(argv, n, 2);
  }
}
