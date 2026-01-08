#include "shell.h"

/**
 * _getenv_var - get the selected enviroment
 * @name: the name of the env variable to get
 *
 * Return: the env variable or NULL
 */
char *_getenv_var(char *name, char **environ)
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

	for (j = 0; env[j] != '\0'; j++)
	{
		if(env[j] == ':')
			i++;
	}

	return (++i);
}

/**
 * create_env_list - create a list from the PATH enviroment variables
 * @name: the name to create a list from
 *
 * Return: pointer to the list
 */
char **create_env_list(char *name, char **environ)
{
	char *env;
	char *token;
	char **list;
	size_t path_num = 0;
	size_t i = 0;

	env = _getenv_var(name, environ);
	path_num = get_num_paths(env);
	list = (char **)malloc(sizeof(char *) * (path_num));

	token = strtok(env, ":");
	while (token)
	{
		list[i++] = token;
		token = strtok(NULL, ":");
	}
	while(*list[0] != '=')
	{
		list[0]++;
	}
	list[0]++;
	list[path_num - 1] = NULL;
	return (list);

}
