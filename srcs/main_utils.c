/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 09:22:37 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/03 15:57:03 by kbrice           ###   ########.fr       */
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

int	exec_builtins(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->name, "cd"))
		ft_cd(((t_tok *)(cmd->args->data))->str);
	/*else if (!ft_strcmp(command[0], "$?"))
		ft_exit_status();*/
	else if (!ft_strcmp(cmd->name, "pwd"))
		ft_pwd();
	// else if (!ft_strcmp(cmd->name, "exit"))
	// 	ft_exit();
	else if (!ft_strcmp(cmd->name, "echo"))
	 	ft_echo(((t_tok *)(cmd->args->data))->str);
	// else if (!ft_strcmp(cmd->name, "env"))
	// 	ft_env(envp);
	else
		return (0);
	return (1);
}
// No longer passes command[1] to cd.
// Works the same way as before, still don't know how to prevent
// double output.