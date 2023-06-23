#include "minishell.h"

int ft_cd(char *path)
{
	char *home_dir;

	if (path == NULL || path[0] == '\0' || path[0] == ' ')
	{
		home_dir = getenv("HOME");
		if (home_dir == NULL)
		{
			perror("Home directory error");
			return (1);
		}
		path = home_dir;
	}
	return (chdir(path));
}

int	pwd(void)
{
	char	*path;

	path = NULL;
	path = getcwd(path, 0);
	if (!path)
	{
		perror(NULL);
		return (CMD_NOT_FOUND);
	}
	printf("%s\n", path);
	free(path);
	return (EXIT_SUCCESS);
}
/* Returns command exit status if the cmd is wrong, otherwise
it prints the path of the current working directory. */

// void	ft_exit_errcode(void)
// {
// 	ft_putstr_fd("exit\n", STDERR_FILENO);
// 	exit(g_exit_status);
// }

// void	ft_exit(char **command)
// {
// 	int argc;

// 	argc = 0;
// 	while (command[argc] != NULL)
// 		argc++;
// 	if (argc == 1)
// 		ft_exit_errcode();
// 	else if (argc == 2 && ft_isdigit_str(command[1])) // Need to create a new function for checking isdigit
// 		g_exit_status = ft_atoi(command[1]);
// 	else if (argc > 2 && ft_isdigit_str(command[1]))
// 	{
// 		printf("exit : %s\n", strerror(errno));
// 		g_exit_status = 1;
// 		return ;
// 	}
// 	else
// 	{
// 		printf("exit : %s\n", strerror(errno));
// 		exit(g_exit_status);
// 	}
// 	ft_exit_errcode();
// }

// int	env(t_env *env)
// {
// 	int	env_exit_status;

// 	print_env_list(env);
// 	return (env_exit_status = 0);
// }