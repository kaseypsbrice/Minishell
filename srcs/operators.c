/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 08:53:34 by kbrice            #+#    #+#             */
/*   Updated: 2023/07/19 14:24:38 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// NOTE: These functions are all WIP

/* Attaches remaining arguments after redirect 
   'echo test > file.txt 123' outputs 'test 123' into file.txt
*/
char	**attach_args(char **dest, char **src)
{
	int		s_len;
	int		d_len;
	int		i;
	char	**temp;

	s_len = -1;
	d_len = -1;
	temp = dest;
	while (src[++s_len])
		;
	while (dest[++d_len])
		;
	dest = (char **)malloc((s_len + d_len + 1) * sizeof(char *));
	if (!dest)
	{
		perror("malloc failed");
		exit(1);
	}
	i = -1;
	while (++i < d_len)
		dest[i] = temp[i];
	i--;
	while (++i < d_len + s_len)
		dest[i] = src[i - d_len];
	dest[i] = NULL;
	free(temp);
	return (dest);
}

// Redirects output to file ">" or ">>"
void	redirect_out(t_mini *cmdline, int *index, int flags)
{
	int	fd;

	//printf("redirecting output\n");
	//print_3d(cmdline->cmd_op);
	if (!cmdline->cmd_op[*index + 2])
	{
		ft_putstr_fd("Minishell: syntax error \
		near unexpected token 'newline'\n", 1);
		exit(1);
	}
	fd = open(cmdline->cmd_op[*index + 2][0], flags, S_IRWXU);
	if (fd == -1)
	{
		ft_putstr_fd("Minishell: could not open file: ", 1);
		ft_putstr_fd(cmdline->cmd_op[*index + 2][0], 1);
		ft_putstr_fd("\n", 1);
		exit(1);
	}
	if (cmdline->cmd_op[*index + 2][1])
		cmdline->cmd_op[*index] = attach_args(cmdline->cmd_op[*index], \
		&(cmdline->cmd_op[*index + 2][1]));
	execute_command(find_command_path(cmdline->cmd_op[*index][0]), \
	cmdline->cmd_op[*index], cmdline->cmd_io[PIPE_READ], fd);
	close(fd);
}

/* Semi-temporary function checks if the next operator is a redirect,
   performs the redirect and returns 1 if the command line should stop being read
*/
int	get_redirect(t_mini *cmdline, int index)
{
	if (!cmdline->cmd_op[index + 1])
		return (0);
	if (ft_strcmp(cmdline->cmd_op[index + 1][0], ">>") == 0)
	{
		redirect_out(cmdline, &index, O_WRONLY | O_CREAT | O_APPEND);
		return (1);
	}
	if (ft_strcmp(cmdline->cmd_op[index + 1][0], ">") == 0)
	{
		redirect_out(cmdline, &index, O_WRONLY | O_CREAT | O_TRUNC);
		return (1);
	}
	return (0);
}
