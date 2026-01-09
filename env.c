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

	if (!name || !environ)
		return (NULL);

	len = strlen(name);

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(name, environ[i], len) == 0 &&
			 environ[i][len] == '=')
		{
			return (environ[i] + len + 1);
		}
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
	size_t i;

	if (!env || env[0] == '\0')
		return (0);

	j = 1;
	for (i = 0; env[i]; i++)
	{
		if (env[i] == ':')
			j++;
	}
	return (j);
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
	char *env_copy;
	char *token;
	char **list;
	size_t path_num = 0;
	size_t i = 0;

	env = _getenv_var(name, environ);

	if (!env || env[0] == '\0')
	{
		list = malloc(sizeof(char *));
		if (!list)
			return (NULL);
		list[0] = NULL;
		return (list);
	}

	path_num = get_num_paths(env);
	list = (char **)malloc(sizeof(char *) * (path_num + 1));
	if (!list)
		return (NULL);
	env_copy = strdup(env);
	if (!env_copy)
	{
		free(list);
		return(NULL);
	}
	token = strtok(env, ":");
	while (token)
	{
		list[i++] = strdup(token);
		token = strtok(NULL, ":");
	}
	list[i] = NULL;
	free(env_copy);
	return (list);

}
