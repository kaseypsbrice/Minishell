/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 09:22:37 by kbrice            #+#    #+#             */
/*   Updated: 2023/07/19 13:11:39 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Recoding builtin checker */
int	exec_builtins(/*t_cmd *cmd, */char **command)
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

void	free_io(char *input, char **command)
{
	free(input);
	//free(command);
}
