#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/**
 * main - the main function, this starts the shell process
 * @argc: number of input arguments
 * @argv: char array of all arguments
 *
 * Return: 0 on success
 */
int main (void)
{
	char *input_line = NULL;
	size_t input_len = 0;

	printf("[H_Shell] $ ");
	while ((getline(&input_line, &input_len, stdin)) != -1)
	{
		printf("%s\n", input_line);
		/*
		 * Break the input_line to an array for strings
		 * This will get our argv[] to pass to execve
		 * NOTE: will need to run fork + wait before execve
		 * fork + wait will keep the shell alive while execve works,
		 * execve will return us to the shell on function termination
		 */
		printf("[H_Shell] $ ");
	}

	free(input_line);
	return (0);
}
