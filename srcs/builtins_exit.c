/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 08:12:40 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/09 08:31:33 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_isdigit_str(char *str)
{
	if (str == NULL)
		return (0);
	while (*str != '\0')
	{
		if (*str < '0' || *str > '9')
			return (false);
		str++;
	}
	return (true);
}

size_t	count_args(char **arr)
{
	size_t	count;

	count = 0;
	while (arr[count])
		count++;
	return (count);
}

int	ft_exit(char **argv)
{
	int	argc;
	int	status;

	argc = count_args(argv);
	if ((argv[1] && ft_isdigit_str(argv[1]) == true))
	{
		status = ft_atoi(argv[1]);
		if (status >= 0 && status <= 255)
		{
			exit(status);
		}
	}
	else if (argc == 1)
	{
		status = g_exit_status;
		exit(status);
	}
	ft_putstr_fd("exit : argument invalid\n", STDERR_FILENO);
	return (EXIT_FAILURE);
}
// After exiting the minishell, you can test that it's exited with
// the correct exit status by using the $? expansion.
