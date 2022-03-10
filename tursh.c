int main() {
  char *argv[3];

  do {
    // input
    if (tursh_input(argv) != 0) {
      // Don't exec
      continue;
    }

    // exec
    if (tursh_exec(argv) != 0) {
      continue;
    }
  } while (1);

  return 0;
}
