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