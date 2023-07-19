/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 08:34:44 by kbrice            #+#    #+#             */
/*   Updated: 2023/07/19 13:41:26 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(char *path)
{
	char	*home_dir;

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
/* If there's no path, then the cd command will return to the home directory. 
 * Othewise, it'll change the directory to the specified path. 
 */

int	ft_pwd(void)
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
 * it prints the path of the current working directory. 
 * 
 * Works perfectly fine.
 */

int	ft_echo(char **command)
{
	int		i;
	bool	flag;

	i = 1;
	flag = false;
	if (command[i] && strcmp(command[i], "-n") == 0)
	{
		flag = true;
		i++;
	}
	while (command[i])
	{
		printf("%s", command[i]);
		if (command[i + 1])
			printf(" ");
		i++;
	}
	if (!flag)
		printf("\n");
	return (EXIT_SUCCESS);
}
/* WIP. Drafted the echo function but don't know whether it actually
 * works, still have to test it.
 */