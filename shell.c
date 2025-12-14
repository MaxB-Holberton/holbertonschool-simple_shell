#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * create_argv - create argv to pass to execve
 * @input: the shell input to break down
 *
 * Return: array of char seperated by the delim
 *
 */
char *create_argv(char *input
{
	char *s = input;
	char *token;
	const char delim[2] = " ";
	char **argv
	int i = 0;

	while (*s != '\0')
	{
		if (*s == ' ')
			i++;
	}
	/*
	 * This will make an array of pointers for char *;
	 */
	argv = malloc(sizeof(char *) * i);
	if (argv == NULL)
		return (NULL);

	token = strtok(input, delim);

	while (input != NULL)
	{
		printf(" %s\n", token);
		token = strtok(NULL, delim);
	}
	return token;
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
	char *argv;
	int i = 0;

	pid_t new_process;

	while (line != -1)
	{
		printf("[H_Shell] $ ");
		/* 
		 * Getline from stdin is blocking, so it won't continue
		 * until \n or EOF is found. 
		 */
		line = getline(&input_line, &input_len, stdin);
		printf("%s\n", input_line);
		argv = create_argv(input_line)

		new_process = fork();
		if (new_process == -1)
		{
			perror("ERROR: ");
			return (1);
		}
		if (child_pid == 0)
		{
			if (execve(argv[0], argv, NULL) == -1)
			{
				perror("ERROR: ");
				return (1);
			}
		}
		/*
		 * Break the input_line to an array for strings
		 * This will get our argv[] to pass to execve
		 * NOTE: will need to run fork + wait before execve
		 * fork + wait will keep the shell alive while execve works,
		 * execve will return us to the shell on function termination
		 */
	}
	/* free argv[] */
	free(input_line);
	return (0);
}
