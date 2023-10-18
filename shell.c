#include "shell.h"

/**
 * get_functions - returns functions for builtin commands
 * @arr: the line splitted into command and args
 *
 * Return: the function that prints the builtin command
 */
int (*get_functions(char **arr))(sh_data *)
{
	built_in sh[] = {
		{"exit", my_exit},
		{"env", my_env},
		{"setenv", my_set},
		{"unsetenv", my_unset},
		{"cd", my_cd},
		{"alias", my_alias},
		{NULL, NULL}
	};

	int j = 0;

	if (arr != NULL)
	{
		while (sh[j].functions != NULL)
		{
			if (my_strcmp(sh[j].str, arr[0]) == 0)
			{
				return (sh[j].functions);
			}
			else
				j++;
		}
	}

	return (NULL);
}

/**
 * non_interact - runs the shell in non interactive mode
 * @shell: the shell data struct
 *
 * Return: non, uses exit with a status
 */
void non_interact(sh_data *shell)
{
	char *path;

	shell->interact = 0;
	for (; ;)
	{
		path = check_shell(shell);
		if (!path && shell->builtin == 0)
			my_exit(shell);
		else if (!path && shell->builtin == 1)
			continue;

		shell->builtin = 0;
		shell->pid = fork();
		if (shell->pid == 0)
		{
			shell->status = execve(path, shell->arr, shell->_environ);
			if (shell->status == -1)
			{
				write(STDERR_FILENO, shell->av[0], my_strlen(shell->av[0]));
				write(STDERR_FILENO, ": ", 2);
				write(STDERR_FILENO, shell->arr[0], my_strlen(shell->arr[0]));
				write(STDERR_FILENO, ": Permission denied\n", 20);
				free(path);
				shell->status = 13;
				my_exit(shell);
			}
		}
		else
		{
			wait(&shell->status);
			shell->status = WEXITSTATUS(shell->status);
		}
		free_arr2(shell->arr);
		free(path);
	}
}

/**
 * loop_shell - runs the shell continously
 * @shell: the shell data
 *
 * Return: void
 */
void loop_shell(sh_data *shell)
{
	char *path;

	if (shell->av[1])
		exit(98);
	else if (!isatty(STDIN_FILENO))
		non_interact(shell);
	else
	{
		for (; ;)
		{
			write(STDOUT_FILENO, "($) ", 4);
			fflush(stdout);
			path = check_shell(shell);
			if (!path)
				continue;
			shell->pid = fork();
			if (shell->pid == 0)
			{
				shell->status = execve(path, shell->arr, shell->_environ);
				if (shell->status == -1)
				{
					write(STDERR_FILENO, shell->av[0], my_strlen(shell->av[0]));
					write(STDERR_FILENO, ": ", 2);
					write(STDERR_FILENO, shell->arr[0], my_strlen(shell->arr[0]));
					write(STDERR_FILENO, ": Permission denied\n", 20);
					free(path);
					shell->status = 13;
					my_exit(shell);
				}
			}
			else
			{
				wait(&shell->status);
				shell->status = WEXITSTATUS(shell->status);
			}
			free_arr2(shell->arr);
			free(path);
		}
	}
}

/**
 * check_shell - checks the shell commands, paths and builtins
 * @shell: pointer to shell data structure
 *
 * Return: path or NULL
 */
char *check_shell(sh_data *shell)
{
	int (*built_in_functions)(sh_data *sh);
	char *path;

	shell->arr = get_commands(shell, shell->line, shell->length);
	if (!shell->arr)
	{
		free(shell->line);
		return (NULL);
	}
	check_alias(shell);
	expand_var(shell);
	built_in_functions = get_functions(shell->arr);

	if (built_in_functions != NULL)
	{
		shell->status = built_in_functions(shell);
		if (shell->arr != NULL)
			free_arr2(shell->arr);
		shell->builtin = 1;
		return (NULL);
	}
	path = search_path(shell->path, shell->arr[0]);

	if (!path)
	{
		write(STDERR_FILENO, shell->av[0], my_strlen(shell->av[0]));
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, shell->arr[0], my_strlen(shell->arr[0]));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		shell->status = 2;
		free_arr2(shell->arr);
		free(path);
		return (NULL);
	}
	return (path);
}

/**
 * main - main body of the shell
 * @ac: command line argument count
 * @av: pointer to strings of command line arguments
 * @env: environment variables
 *
 * Return: 0 at successful exit
 */
int main(int ac, char *av[], char *env[])
{
	sh_data shell;
	int j;

	shell.line = NULL;
	shell.length = 0;
	shell.interact = 1;
	shell.pid = getpid();
	shell.status = 0;
	shell.builtin = 0;
	shell.arr = NULL;
	shell.alias = NULL;
	(void) env;

	shell.av = malloc(sizeof(char *) * (ac + 1));
	for (j = 0; av[j]; j++)
		shell.av[j] = my_strdup(av[0]);
	shell.av[j] = NULL;

	/* count th enumber of env strings to malloc a copy */
	for (j = 0; environ[j]; j++)
		;

	shell._environ = malloc(sizeof(char *) * (j + 1));
	for (j = 0; environ[j]; j++)
		shell._environ[j] = my_strdup(environ[j]);
	shell._environ[j] = NULL;

	shell.path = path_to_list(&shell);

	loop_shell(&shell);

	return (0);
}
