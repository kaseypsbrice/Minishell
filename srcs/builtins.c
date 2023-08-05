/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 08:34:44 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/03 15:57:50 by kbrice           ###   ########.fr       */
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
 */

int	ft_echo(char **command)
{
	int		i;
	int		j;
	bool	n_option;

	i = 1;
	n_option = false;
	while (command[i] && command[i][0] == '-' && command[i][1] == 'n')
	{
		j = 1;
		while (command[i][j] == 'n')
			j++;
		if (command[i][j] == '\0')
		{
			n_option = true;
			i++;
		}
		else
			break ;
	}
	while (command[i] && (printf("%s", command[i]), \
	command[i + 1] && printf(" "), i++))
		;
	if (!n_option)
		printf("\n");
	return (EXIT_SUCCESS);
}