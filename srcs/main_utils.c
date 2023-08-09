/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 09:22:37 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/09 12:19:55 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_empty(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (parse_type(str[i]) != P_SPACE)
			return (0);
	}
	return (1);
}

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
	if (!ft_strcmp(command, "cd") || \
		!ft_strcmp(command, "pwd") || \
		!ft_strcmp(command, "echo") || \
		!ft_strcmp(command, "env") || \
		!ft_strcmp(command, "export"))
		return (1);
	return (0);
}

int	exec_builtins(t_cmd *cmd, t_list *envvar_list)
{
	if (!ft_strcmp(cmd->name, "cd"))
		return (ft_cd(cmd->argv, envvar_list));
	else if (!ft_strcmp(cmd->name, "pwd"))
		return (ft_pwd(cmd->argv));
	else if (!ft_strcmp(cmd->name, "exit"))
		return (ft_exit(cmd->argv));
	else if (!ft_strcmp(cmd->name, "echo"))
		return (ft_echo(cmd->argv));
	else if (!ft_strcmp(cmd->name, "env"))
		return (ft_env(cmd->argv, envvar_list));
	else if (!ft_strcmp(cmd->name, "export"))
		return (ft_export(cmd->argv, &envvar_list));
	else if (!ft_strcmp(cmd->name, "unset"))
		return (ft_unset(cmd->argv, &envvar_list));
	free(envvar_list);
	return (1);
}
