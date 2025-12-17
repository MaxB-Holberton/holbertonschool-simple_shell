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
	size_t argc = 0, size = 8;
	char **tmp;

	if (input == NULL)
		return (NULL);

	argv = malloc(sizeof(char *) * size);
	if (!argv)
		return (NULL);

	token = strtok(input, " \t\n");
	while (token)
	{
		argv[argc++] = token;

		if (argc >= size)
		{
			size *= 2;
			tmp = realloc(argv, sizeof(char *) * size);
			if (!tmp)
			{
				free (argv);
				return (NULL);
			}
			argv = tmp;
		}
		token = strtok(NULL, " \t\n");
	}

	argv[argc] = NULL;
	return argv;
}

/**
 * trim_spaces - removes leading and trailing spaces from a string
 * @str: input string
 *
 * Return: pointer to trimmed string
 */
char *trim_spaces(char *str)
{
	char *end;

	while (*str == ' ' || *str == '\t')
		str++;

	if (*str == '\0')
		return (str);

	end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t' || *end == '\n'))
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
	ssize_t line_len;
	int status;
	pid_t new_process;
	char **argv;
	char *trimmed;

	while (1)
	{
		if (isatty(STDIN_FILENO)) /* checks if open file refers to terminal */
			printf("[H_Shell] $ ");
		/* will stop it from printing [H_Shell] $ to output */

		line_len = getline(&input_line, &input_len, stdin);
		if (line_len == -1)
		{
			if (isatty(STDIN_FILENO))
				printf("\n");
			break;
		}

		char *trimmed = trim_spaces(input_line);
		if (*trimmed == '\0')
			continue;

		char **argv = create_argv(trimmed);
		if (!argv)
			continue;

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
