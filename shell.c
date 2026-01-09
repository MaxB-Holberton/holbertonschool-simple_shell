#include "shell.h"

extern char **environ;

/**
 * new_fork - checks PATH and create a new process
 * @argv: the list of commands to execute
 *
 * Return: 0 - parent return | -1 - execve fails
 */
int new_fork(char **argv)
{
	pid_t new_process;
	int status;

	new_process = fork();
	if (new_process == -1)
	{
		return (-1);
	}
	if (new_process == 0)
	{
		execve(argv[0], argv, environ);
		_exit(127);
		/* should only return on failed call */
	}
	if (waitpid(new_process, &status, 0) == -1)
		return (-1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));

	return (0);
}

/**
 * check_path - checks the Path for the requested exe file
 * @argv: the list of arguments
 * @path_list: the PATH env as a list
 *
 * Return: 0 success | -1 if new_fork child fails | 1 on generic fail
 */
int check_path(char **argv, char **path_list)
{
	size_t i = 0;
	size_t len = 0;
	char *full_path;

	if (strchr(argv[0], '/') != NULL)
	{
		/* checking if the command already contains a path */
		if (access(argv[0], X_OK) == 0)
		{
			return(new_fork(argv));
		}
		return (1);
	}
	if (!path_list || !path_list[0])
		return (1);

	for (i = 0; path_list[i]; i++)
	{
		/* malloc each path_list item w/ argv_0 */
		len = strlen(path_list[i]) + strlen(argv[0]) + 2;
		full_path = malloc(len);
		if (!full_path)
			return (1);

		strcpy(full_path, path_list[i]);
		strcat(full_path, "/");
		strcat(full_path, argv[0]);

		if (access(full_path, X_OK) == 0)
		{
			argv[0] = full_path;
			new_fork(argv);
			free(full_path);
			return(rtn);
		}
		free(full_path);
	}
	return (1);
}
/**
 * pre_process - handles reserved argv_0 lines
 * @argv: the arguments
 * @path_list: the list of env PATHs
 *
 * Return: 0 on sucess
 */
int pre_process(char **argv, char** path_list)
{
	unsigned int i = 0;
	if (argv[0] == NULL)
	{
		return(0);
	}
	if (strcmp(argv[0], "exit") == 0)
	{
		return (2);
	}
	if (strcmp(argv[0], "env") == 0)
	{
		while (environ[i] != NULL)
		{
			printf("%s\n", environ[i]);
			i++;
		}
		return (0);
	}
	return (check_path(argv, path_list));
}

/**
 * main - the main function, starts the shell process
 *
 * Return: 0 on success
 */
int main(void)
{
	char *input_line = NULL;
	size_t input_len = 0;
	ssize_t line_len;
	char **argv;
	char **path_list;
	int status = 0;
	size_t i;

	path_list = create_env_list("PATH", environ);
	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			printf("[H_Shell]$ ");
		}
		line_len = getline(&input_line, &input_len, stdin);
		if (line_len == -1)
		{
			/* EoF reached */
			if (isatty(STDIN_FILENO))
				printf("\n");

			break;
		}
		/* trim the string and then create argv */
		argv = create_argv(trim_string(input_line, line_len));
		if (argv == NULL)
		{
			continue;
		}
		status = pre_process(argv, path_list);
		if (status == -1 || status == 2)
		{
			/* if 'exit' or child failed to execve */
			free(argv);
			break;
		}
		if (status == 1)
		{
			fprintf(stderr, "./hsh: 1: %s: not found\n", argv[0]);
			status = 127;
			free(argv);
			break;
		}
		free(argv);
	}
	if (path_list)
	{
		for (i = 0; path_list[i]; i++)
			free(path_list[i]);
		free(path_list);
	}
	free(input_line);
	if (status == -1)
	{
		perror("ERROR");
		return (1);
	}
	return (status);
}
