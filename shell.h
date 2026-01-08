#ifndef SHELL_H
#define SHELL_H

/* Libraries (add to when needed) */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>

/* We can also put function in here later if needed too */
char *trim_string(char *input, ssize_t end);
char **create_argv(char *input);
char **create_env_list(char *name, char **environ);


#endif /* SHELL_H */
