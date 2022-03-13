#ifndef INPUT_H_
#define INPUT_H_

char** tursh_input();

int search_argv(char* argv[], char* arg);

void delete_argv(char* argv[], int pos, int n);

int get_argc(char* argv[]);

void extract_argv(char* dst[], char* src[], int pos, int n);

void dump_argv(char* name, char** argv);

#endif  // INPUT_H_
