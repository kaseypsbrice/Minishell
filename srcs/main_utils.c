/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 09:22:37 by kbrice            #+#    #+#             */
/*   Updated: 2023/07/25 12:48:01 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	ft_strjoin but you can choose which string to free	
	0 = first, 1 = second, >1 = both	*/
char	*ft_strjoinf(char *s1, char *s2, int which)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	if (which == 0 || which > 1)
		free(s1);
	if (which >= 1)
		free(s2);
	return (res);
}

int	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	if (S_ISDIR(statbuf.st_mode))
		return (1);
	return (0);
}

int	is_builtin(char *command)
{
	if (!ft_strcmp(command, "cd") ||
		!ft_strcmp(command, "pwd") ||
		!ft_strcmp(command, "echo"))
			return (1);
	return (0);
}

int	exec_builtins(char **command)
{
	if (!ft_strcmp(command[0], "cd"))
		ft_cd(command[1]);
	/*else if (!ft_strcmp(command[0], "$?"))
		ft_exit_status();*/
	else if (!ft_strcmp(command[0], "pwd"))
		ft_pwd();
	/*else if (!ft_strcmp(command[0], "exit"))
		ft_exit(command[1]);*/
	else if (!ft_strcmp(command[0], "echo"))
		ft_echo(command[1]);
	else
		return (0);
	return (1);
}
/* Where should we run this function though? 
 */