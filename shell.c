#include "shell.h"

/**
 * check_path - check the paths for the execuatable file
 * @argv: the arguments
 * @env_list: the path list
 *
 * Return: 0 on success or 1 on failure
 */
int check_path(char **argv, char **env_list)
{
	size_t i = 0;
	size_t len;
	char *full_path;

	/* checking if the command already contains a path */
	if (strchr(argv[0], '/') != NULL)
	{
		if (access(argv[0], X_OK) == 0)
			return (0);
		return (1);
	}

	/* searching in PATH */
	for (i = 0; env_list[i] != NULL; i++)
	{
		len = strlen(env_list[i]) + strlen(argv[0]) + 1;
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
			return (0);
		}
	}
	free(full_path);
	return (1);
}

/**
 * create_process - checks PATH and create a new process
 * @argv: the list of commands to execute
 *
 * Return: 0 without calling execve | 1 if execve fails
 */
int create_process(char **argv, char **env_list)
{
	pid_t new_process;
	int status;

	if (check_path(argv, env_list) == 1)
	{
		printf("ERROR: %s cannot be found\n", argv[0]);
		return (0);
	}
	new_process = fork();
	printf("new process found\n");
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
	char **env_list;

	env_list = create_env_list("PATH");
	while (1)
	{
		if (!isatty(STDIN_FILENO))
		{
			/* if the file descriptor is not stdin */
			perror("ERROR: not reading STDIN");
			return (1);
		}
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

		if (create_process(argv, env_list) == 1)
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
