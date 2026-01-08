#include "shell.h"

/**
 * trim_string - removes excess spaces, tabs and newlines from a string
 * @input: the input to clean up
 * @len: the length of the input, used to ensure '\0' is included
 *
 * Return: pointer to trimmed string
 */
char *trim_string(char *input, ssize_t end)
{
	ssize_t i = 0;
	ssize_t j = 0;

	if (input == NULL || input[i] == '\0')
	{
		return (NULL);
	}

	/* remove whitespace before text body */
	while (!(input[i] > 32 && input[i] < 127) && (i < end))
	{
		i++;
	}

	if (i == end)
	{
		return (NULL);
	}
	/* remove whitespace after text body */
	while (!(input[end - 1] > 32 && input[end - 1] < 127))
		end--;

	/* remove whitespace in the middle of the text body */
	for (; i <= end; i++)
	{
		if ((input[i] >= 32) && (input[i] < 127))
		{
			if (input[i] == 32 && !((input[i + 1] > 32) && (input[i + 1] < 127)))
			{
				continue;
			}
			input[j++] = input[i];

		}
	}
	input[j] = '\0';
	return (input);
}

/**
 * create_argc - find the number of arguments for argv
 * @input: the string to create argc with
 *
 * Return: unsigned int of the number of arguments
 */
size_t create_argc(char *input)
{
	size_t j = 0;
	size_t i = 1; /* minimum # args will be 1 */

	for (j = 0; input[j] != '\0'; j++)
	{
		if(input[j] == ' ')
			i++;
	}

	return (++i);
}

/**
 * create_argv - create argv to pass to execve
 * @input: the shell input to break down
 *
 * Return: pointer to a pointer of strings (argv)
 */
char **create_argv(char *input)
{
	char *token;
	char **argv;
	size_t i = 0;
	size_t argc = 0;

	if (input == NULL)
	{
		return (NULL);
	}
	argc = create_argc(input);
	argv = (char **)malloc(sizeof(char *) * (argc));
	if (argv == NULL)
	{
		return (NULL);
	}
	token = strtok(input, " ");
	while (token)
	{
		argv[i++] = token;
		token = strtok(NULL, " ");
	}
	argv[argc - 1] = NULL;

	return argv;
}
