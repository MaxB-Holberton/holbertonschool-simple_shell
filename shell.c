#include "shell.h"

extern char **environ;

/**
 * create_argv - create argv to pass to execve
 * @input: the shell input to break down
 *
 * Return: pointer to a pointer of strings (argv)
 *
 */
char **create_argv(char *input)
{
	char *token;
	char **argv;

	argv = malloc(sizeof(char *) * 2);
	if (argv == NULL)
		return (NULL);

	token = strtok(input, "\n");
	if (token == NULL)
	{
		free(argv);
		return (NULL);
	}

	argv[0] = token;
	argv[1] = NULL;
	return argv;
}

/**
 * trim_spaces - deals with trailing and leading spaces
 * @str: pointer to string
 * Return: string on success
 */
char *trim_spaces(char *str)
{
	char *end;

	while (*str == ' ')
		str++;

	if (str == '\0')
		return (str);

	end = str + strlen(str) - 1;
	while (end > str && *end == ' ')
		end--;

	end[1] = '\0';

	return (str);
}

/**
 * main - the main function, starts the shell process
 *
 * Return: 0 on success
 */
int main (void)
{
	char *input_line = NULL;
	size_t input_len = 0;
	int line = 0;
	char **argv;
	int status;
	pid_t new_process;

	while (1)
	{
		if (isatty(STDIN_FILENO)) /* checks if open file refers to terminal */
			printf("[H_Shell] $ ");
		/* will stop it from printing [H_Shell] $ to output */

		line = getline(&input_line, &input_len, stdin);
		if (line == -1)
		{
			if (isatty(STDIN_FILENO))
				printf("\n");
			break;
		}

		argv = create_argv(input_line);
		if (argv == NULL)
		{
			continue;
		}

		new_process = fork();
		if (new_process == -1)
		{
			perror("ERROR");
			free(argv);
			continue;
		}
		if (new_process == 0)
		{
			execve(argv[0], argv, environ);
			perror("ERROR");
			exit(1);
		}
		else
		{
			wait(&status);
			free(argv);
		}
	}
	free(input_line);
	return (0);
}
