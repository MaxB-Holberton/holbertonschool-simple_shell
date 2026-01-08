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
	if (new_process == -1)
	{
		return (1);
	}
	if (new_process == 0)
	{
		execve(argv[0], argv, environ);
		perror("ERROR - failed child");
		return (1);
		/* should only return on failed call */
	}
	waitpid(new_process, &status, 0);
	return (0);
}

/**
 * handle_exit - handles exit built-in
 * @argv: argv
 * @input_line: input buffer
 * @env_list: PATH list
 *
 * Return: 1 on exit, 0 otherwise
 */
int handle_exit(char **argv, char *input_line, char **env_list)
{
	int status = 0;
	char *end

	if (argv[1])
	{
		status = strtol(argv[1], &end, 10);
		if (*end != '\0')
		{
			printf("exit: numeric argument required\n");
			status = 2;
		}
		else if (argv[2])
		{
			printf("exit: too many arguments\n");
			return (0);
		}
	}
free(argv[0]);
free(argv);
free(input_line);
free(env_list);

exit(status);
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
	char *argv_0_dup;
	int rtn = 0;

	if(!argv || !argv[0])
		return (0);

	/*
	 * TODO: 'env' handling here
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
			argv[0] = full_path;
			rtn = new_fork(argv);
			free(full_path);
			full_path = NULL;
			return(rtn);
		}
	}
	free(full_path);
	full_path = NULL;
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
		if (isatty(STDIN_FILENO))
		{
			printf("[H_Shell]$ ");
		}
		line_len = getline(&input_line, &input_len, stdin);
		if (line_len == -1)
		{
			/* EoF reached */
			printf("\n");
			break;
		}
		if (strcmp(argv[0], "exit") == 0) /* exit handling main */
			handle_exit(argv, input_line, env_list);
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
			perror("ERROR");
			free(argv);
			free(argv[0]);
			free(input_line);
			free(env_list);
			return (1);
		}
		free(argv[0]);
		free(argv);
	}
	free(env_list);
	free(input_line);
	return (0);
}
