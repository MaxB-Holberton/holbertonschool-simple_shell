#include "shell.h"

/**
 * create_process - checks PATH and create a new process
 * @argv: the list of commands to execute
 *
 * Return: 0 on success | 1 if execve fails
 */
int create_process(char **argv)
{
	pid_t new_process;
	int status;

	/*
	 * TODO: check to see if file requested is in PATH
	 * Success: Fork process and run
	 * Failure: Return 0;
	 */
	new_process = fork();
	if (new_process > 0)
	{
		wait(&status);
		free(argv);
		return (0);
	}
	if (new_process == 0)
	{
		execve(argv[0], argv, NULL);
	}
	return (1);
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

	while (1)
	{
		if (!isatty(STDIN_FILENO))
		{
			/* if the file descriptor is not stdin */
			perror("ERROR: not reading STDIN");
			return (1);
		}
		/*
		 * TODO: create return variable for create_env_list
		 */
		create_env_list("PATH");
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

		if (create_process(argv) == 1)
		{
			/* If child fails to execute */
			perror("ERROR");
			free(argv);
			free(input_line);
			return (1);
		}
	}
	free(input_line);
	return (0);
}
