/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 10:54:38 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/07 10:54:41 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	command_not_found(char *name)
{
	ft_putstr_fd("\'", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd("\' command not found\n", 1);
	return (127);
}

int	command_is_directory(char *name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": Is a directory\n", 1);
	return (1);
}

void	perror_exit(const char *msg, int status)
{
	perror(msg);
	exit(status);
}

/*	Prints error message. If errno = 'exec format error' then
	display 'command not found' instead.
	This is to try mimic bash, it's a little messy	*/
int	display_errno(char *str)
{
	int	errnum;

	errnum = errno;
	if (errnum == 8)
		str = find_command_name(str);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": ", 2);
	if (errnum == 8)
		ft_putstr_fd("command not found", 2);
	else
		ft_putstr_fd(strerror(errnum), 2);
	ft_putstr_fd("\n", 2);
	g_exit_status = errnum;
	return (1);
}

char	*invalid_syntax(char c)
{
	if (c == '\n')
		ft_putstr_fd("minishell: syntax error near \
unexpected token 'newline'\n", 2);
	else
	{
		ft_putstr_fd("minishell: syntax error near \
unexpected token '", 2);
		ft_putchar_fd(c, 2);
		ft_putstr_fd("'\n", 2);
	}
	g_exit_status = 2;
	return (NULL);
}
