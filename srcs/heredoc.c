/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 11:12:13 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/07 11:12:15 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	<< operator. Reads lines from standard input until line = eof	*/
int	heredoc(t_cmd *cmd, char *eof)
{
	int		fd[2];
	char	*buf;

	if (pipe(fd) != 0)
		perror_exit("pipe_failed", 1);
	close(cmd->fd_in);
	cmd->fd_in = fd[PIPE_READ];
	while (1)
	{
		ft_putstr_fd("> ", STDIN_FILENO);
		buf = get_next_line(STDIN_FILENO);
		if (ft_strncmp(buf, eof, ft_strlen(buf) - 1) == 0)
		{
			free(buf);
			close(fd[PIPE_WRITE]);
			return (0);
		}
		ft_putstr_fd(buf, fd[PIPE_WRITE]);
	}
	return (1);
}
/*	Surprisingly easy to implement.
	Opens a new pipe to act as the here document.
	Reads from stdin using get_next line and writes it to
	the heredoc if it doesn't match the eof string.
	ft_strlen(buf) - 1 is used to disregard the newline.	*/
