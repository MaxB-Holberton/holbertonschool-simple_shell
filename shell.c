#include "shell.h"

/**
 * check_path - check the paths for the execuatable file
 * @argv: the arguments
 * @env_list: the path list
 *
 * Return: 0 on success or -1 on failure
 */
int check_path(char **argv __attribute__((unused)), char **env_list)
{
	/*DIR *dir;
	struct dirent *current_dir;*/
	size_t i = 0;
	/*
	 * TODO: find the path and insert into argv[0];
	 * If argv[0] starts with /xxx - goto path and find file instead
	 * If not starting with /xxx or file at /xxx not found - find it in PATH
	 * Return 0 if found or -1 if not found
	 */
	while(env_list[i] != NULL)
	{
		/*dir = opendir(env_list[i]);
		if (dir == NULL)
		{
			printf("this dir not found\n");
			continue;
		}*/
		printf("%s\n", env_list[i]);
		i++;

		/* read the entry
		while (current_dir = readdir(dir) != NULL)
		{
			if (strcmp(current_dir->d_name, argv[0]) == 0)
			{

			}
		}*/
	}

	return (-1);
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

	/*
	 * TODO: check to see if file requested is in PATH
	 * Success: Fork process and run
	 * Failure: Return 0;
	 */
	if (check_path(argv, env_list) == 0)
	{
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
			perror("ERROR");
			free(argv);
			free(input_line);
			return (1);
		}
	}
	free(input_line);
	return (0);
}
