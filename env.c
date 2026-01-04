#include "shell.h"

extern char **environ;

/**
 * _getenv_var - get the selected enviroment
 * @name: the name of the env variable to get
 *
 * Return: the env variable or NULL
 */
char *_getenv_var(char *name)
{
	int i = 0;
	int len = 0;

	len = strlen(name);

	while (environ[i])
	{
		if (strncmp(name, environ[i], len) == 0)
		{
			return (environ[i]);
		}
		i++;
	}
	return (NULL);
}

/**
 * get_num_paths - get the number of paths from env
 * @env: the env
 *
 * Return: The number of paths
 */
size_t get_num_paths(char *env)
{
	size_t j = 0;
	size_t i = 1; /* minimum # paths will be 1 */

	for (j = 0; input[j] != '\0'; j++)
	{
		if(input[j] == ':')
			i++;
	}

	return (++i);
}

/**
 * create_env_list - create a linked list from
 * @name: the name to create a list from
 *
 * Return: pointer to the list
 */
char *create_env_list(char *name)
{
	char *env;
	char **list;
	size_t paths;

	env = _getenv_var(name);
	paths = get_num_paths(env);
	/*
	 * TODO: create a list
	 * each list item is the path seperated by ":"
	 * return the list back
	 */

}
