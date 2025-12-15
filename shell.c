#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/**
 * create_argv - create argv to pass to execve
 * @input: the shell input to break down
 *
 * Return: pointer to a pointer of strings (argv)
 *
 */
char **create_argv(char *input)
{
	char *s = input;
	char *token;
	const char delim[2] = " ";
	char **argv;
	unsigned int len = 0;
	int i = 0;

	while (*s != '\0')
	{
		if (*s == ' ')
		{
			len++;
		}
		s++;
	}

	argv = malloc(sizeof(char *) * len);
	if (argv == NULL)
	{
		return (NULL);
	}
	token = strtok(input, delim);
	while (token != NULL)
	{
		/*
		 * Edit the token to remove all non characters (i.e \n '\0')
		 */
		argv[i++] = token;
		printf("%ld |%ld\n ", strlen(token), sizeof(token));
		token = strtok(NULL, delim);

	}
	return argv;
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

	while (line != -1)
	{
		printf("[H_Shell] $ ");
		/* 
		 * Getline is blocking, so it won't continue
		 * until \n or EOF is written to stdin.
		 */
		line = getline(&input_line, &input_len, stdin);
		printf("%s\n", input_line);

		argv = create_argv(input_line);
		if (argv == NULL)
		{
			perror("ERROR: ");
			return (1);
		}

		new_process = fork();
		if (new_process == -1)
		{
			perror("ERROR: ");
			return (1);
		}
		if (new_process == 0)
		{
			if (execve(argv[0], argv, NULL) == -1)
			{
				perror("ERROR: ");
				return (1);
			}
		}
		else
		{
			wait(&status);
		}
	}
	/* free argv[] */
	free(input_line);
	return (0);
}
