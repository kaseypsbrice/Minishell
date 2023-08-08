/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 08:34:44 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/09 09:00:14 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(char **argv)
{
	if (!argv[1] || !argv[1][0] || argv[1][0] == ' ')
	{
		argv[1] = getenv("HOME");
		if (argv[1] == NULL)
		{
			perror("Home directory error");
			return (1);
		}
	}
	else if (chdir(argv[1]) == -1)
	{
		ft_putstr_fd("cd : invalid path\n", STDERR_FILENO);
		return (1);
	}
	return (chdir(argv[1]));
}
/* If there's no path, then the cd command will return to the home directory. 
 * Othewise, it'll change the directory to the specified path. 
 * If it fails to change directories, it'll produce an error message.
 */
// [!] Suddenly Home is not set ??

int	ft_pwd(char **argv)
{
	char	*path;

	path = NULL;
	path = getcwd(path, 0);
	if (!path || count_args(argv) > 1)
	{
		perror(NULL);
		return (CMD_NOT_FOUND);
	}
	printf("%s\n", path);
	free(path);
	return (EXIT_SUCCESS);
}
/* Returns command exit status if the cmd is wrong, otherwise
 * it prints the path of the current working directory. 
 */

static bool	echo_n_option(char *str)
{
	int	j;

	j = 1;
	while (str[j] == 'n')
		j++;
	return (str[j] == '\0');
}

static void	print_echo_args(char **args, bool n_option)
{
	int	i;

	i = 1;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!n_option)
		printf("\n");
}

int	ft_echo(char **command)
{
	int		i;
	bool	n_option;

	i = 1;
	n_option = false;
	if (command[i] == NULL)
	{
		write(1, "\n", 1);
		return (EXIT_SUCCESS);
	}
	while (command[i] && command[i][0] == '-' && command[i][1] == 'n')
	{
		if (echo_n_option(command[i]))
		{
			n_option = true;
			i++;
		}
		else
			break ;
	}
	print_echo_args(command, n_option);
	return (EXIT_SUCCESS);
}
