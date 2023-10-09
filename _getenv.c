#include "shell.h"

/**
 * _getenv - gets the value of an environment variable
 * @shell: shell data struct (shell._environ contains all env data)
 * @name: name of variable to return
 *
 * Return: pointer to the value of env
 */
char *_getenv(sh_data *shell, char *name)
{
	int j, k, l, m, check;
	char *value;

	for (j = 0; shell->_environ[j]; j++)
	{
		k = 0, check = 0;
		while (shell->_environ[j][k] != '=' && name[k] != '\0')
		{
			if (shell->_environ[j][k] != name[k])
				check = 1;
			k++;
		}
		if (check == 0 && shell->_environ[j][k] == '=')
		{
			for (l = k + 1, m = 0; shell->_environ[j][l]; l++, m++)
				;

			value = malloc(sizeof(char) * (m + 1));

			for (l = k + 1, m = 0; shell->_environ[j][l]; m++, l++)
				value[m] = shell->_environ[j][l];
			value[m] = '\0';
			return (value);
		}
	}

	return (NULL);
}
