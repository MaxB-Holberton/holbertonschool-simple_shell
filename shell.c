#include "shell.h"

extern char **environ;

/**
 * new_fork - checks PATH and create a new process
 * @argv: the list of commands to execute
 *
 * Return: 0 - parent return | 1 - execve fails
 */
int new_fork(char **argv)
{
	pid_t new_process;
	int status;

	new_process = fork();
	if (new_process > 0)
	{
		wait(&status);
		free(argv);
		return (0);
	}
	if (new_process == 0)
	{
		execve(argv[0], argv, environ);
		/* should only return on failed called */
	}
	return (1);
}

/**
 * check_path - checks the Path for the requested exe file
 * @argv: the list of arguments
 * @env_list: the PATH env as a list
 *
 * Return: 0 if new_fork returns from parent | 1 if new_fork child fails
 */
int check_path(char **argv, char **env_list)
{
	size_t i = 0;
	size_t len = 0;
	size_t argv_0_len = 0;
	char *full_path;
	int rtn = 0;

	/*
	 * TODO: add 'exit' & 'env' handling here
	 */
	if (strchr(argv[0], '/') != NULL)
	{
		/* checking if the command already contains a path */
		if (access(argv[0], X_OK) == 0)
		{
			return(new_fork(argv));
		}
		printf("ERROR: %s not found\n", argv[0]);
		return (0);
	}
	argv_0_len = strlen(argv[0]) + 1;
	/* searching in PATH */
	for (i = 0; env_list[i] != NULL; i++)
	{
		len = strlen(env_list[i]) + argv_0_len;
		full_path = malloc(len);
		if (!full_path)
			return (1);

		strcpy(full_path, env_list[i]);
		strcat(full_path, "/");
		strcat(full_path, argv[0]);
		if (access(full_path, X_OK) == 0)
		{
			/*free(argv[0]);*/
			argv[0] = full_path;
			rtn = new_fork(argv);
			free(full_path);
			full_path = NULL;
			return(rtn);
		}
		free(full_path);
	}
	free(full_path);
	printf("ERROR: %s not found\n", argv[0]);
	return (0);
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
	char **env_list;
	int process_status = 0;

	env_list = create_env_list("PATH", environ);
	while (1)
	{
		/*
		 * this is causing issues with non-interactive mode
		if (!isatty(STDIN_FILENO))
		{
			if the file descriptor is not stdin
			perror("ERROR: not reading STDIN");
			return (1);
		}*/
		printf("[H_Shell]$ ");
		line_len = getline(&input_line, &input_len, stdin);
		if (line_len == -1)
		{
			/* EoF reached */
			printf("\n");
			break;
		}
		/* trim the string and then create argv */
		argv = create_argv(trim_string(input_line, line_len));
		if (argv == NULL)
		{
			continue;
		}
		if (argv[0] == NULL)
		{
			continue;
		}
		process_status = check_path(argv, env_list);
		if (process_status == 1)
		{
			/* If child fails to execute */
			perror("ERROR - failed child");
			free(argv);
			free(input_line);
			free(env_list);
			return (1);
		}
	}
	free(env_list);
	free(input_line);
	return (0);
}
