#ifndef ARGV_H_
#define ARGV_H_

int get_argc(char* argv[]);

int search_argv(char* argv[], char* arg);
void delete_argv(char* argv[], int pos, int size);
void extract_argv(char* dst[], char* src[], int pos, int size);

void dump_argv(char* name, char** argv);

void free_argv(char** argv);

#endif  // ARGV_H_
