#ifndef UTILS_H_
#define UTILS_H_

#define PATH_TOKEN_MAX  32
#define COMMAND_LEN_MAX 64
#define PATH_LEN_MAX    256

char* get_envp_path();
char** get_env_list();

#endif  // UTILS_H_
