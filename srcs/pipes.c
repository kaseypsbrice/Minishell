/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 09:22:09 by kbrice            #+#    #+#             */
/*   Updated: 2023/07/19 13:24:56 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Opens a new right pipe
int	update_pipes(t_mini *cmdline, int pipes_passed)
{
	if (pipe(cmdline->pipes[PIPE_RIGHT]) == -1) //open new pipe
	{
		perror("Pipe failed");
		exit(1);
	}
	//printf("opening: %d\nfd read: %d fd write: %d\n", PIPE_RIGHT, cmdline->pipes[PIPE_RIGHT][PIPE_READ], cmdline->pipes[PIPE_RIGHT][PIPE_WRITE]);
	return (pipes_passed);
}

int	init_pipes(t_mini *cmdline)
{
	if (pipe(cmdline->pipes[0]) == -1 || pipe(cmdline->pipes[1]) == -1)
	{
		perror("Pipe failed");
		exit(1);
	}
	return (0);
}

int try_open(t_cmd *cmd, t_tok *redir)
{
	int	fd;

	if (redir->type == R_OUTPUT)
		fd = open(redir->str, O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
	else if (redir->type == R_OUTPUT_A)
		fd = open(redir->str, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
	else if (redir->type == R_INPUT)
		fd = open(redir->str, O_RDONLY);
	else
	{
		//heredoc
		return (0);
	}
	if (fd == -1)
		return (display_errno(redir->str));
	if (redir->type == R_INPUT)
		cmd->fd_in = fd;
	else
		cmd->fd_out = fd;
	return (0);
}

int	handle_redirects(t_cmd *cmd)
{
	t_list	*cur;

	cur = cmd->redirs;
	while (cur)
	{
		if (try_open(cmd, (t_tok *)cur->data))
			return (1);
		cur = cur->next;
	}
	return (0);
}

/* It's called handle pipes but really its handle pipes, handle quotes, handle expansions and run the command line.
   Loops through the mess created in cmd_op_tab to set pipe in and pipe out for execute_command().
   Tested with "ping google.com -c 5 | grep rtt | wc | cat -e".
*/
void	handle_pipes(t_mini *cmdline)
{
	t_list	*cur;
	t_cmd	*cmd;
	int		i;

	i = init_pipes(cmdline);
	cur = cmdline->cmds;
	while (cur)
	{
		cmd = (t_cmd *)cur->data;

		if (handle_redirects(cmd))
			return ;
		if (cmd->fd_out == -1 && cur->next)
			cmd->fd_out = cmdline->pipes[i % 2][PIPE_WRITE];
		if (cur->next)
			((t_cmd *)cur->next->data)->fd_in = cmdline->pipes[i % 2][PIPE_READ];
		execute_command(cmd->path, cmd->argv, cmd->fd_in, cmd->fd_out);
		close(cmdline->pipes[i % 2][PIPE_WRITE]);
		i = update_pipes(cmdline, i + 1);
		cur = cur->next;
	}
	close(cmdline->pipes[i % 2][PIPE_READ]); 
	if (i > 0)
		close(cmdline->pipes[i % 2 + 1][PIPE_WRITE]);
}
/*	Right Pipe = (i % 2)
	Left Pipe = (i % 2 + 1)	*/

/*void	handle_pipes(t_mini *cmdline, char *input)
{
	int		i;
	int		pipes_passed;

	i = -1;
	pipes_passed = init_pipes(cmdline);
	cmdline->cmd_op = cmd_op_tab(input);
	while (cmdline->cmd_op[++i])
	{
		if (ft_strcmp(cmdline->cmd_op[i][0], "|") == 0)
		{
			pipes_passed = update_pipes(cmdline, pipes_passed + 1);
			continue ;
		}
		cmdline->cmd_io[PIPE_READ] = -1;
		cmdline->cmd_io[PIPE_WRITE] = -1;
		//printf("\npipe_right: read %d write %d\n", cmdline->pipes[PIPE_RIGHT][PIPE_READ], cmdline->pipes[PIPE_RIGHT][PIPE_WRITE]);
		//if (pipes_passed > 0)
		//	printf("\npipe_left: read %d write %d\n", cmdline->pipes[PIPE_LEFT][PIPE_READ], cmdline->pipes[PIPE_LEFT][PIPE_WRITE]);
		if (i > 0 && pipes_passed > 0 && ft_strcmp(cmdline->cmd_op[i - 1][0], "|") == 0)
			cmdline->cmd_io[PIPE_READ] = cmdline->pipes[PIPE_LEFT][PIPE_READ];
		if (cmdline->cmd_op[i + 1] && ft_strcmp(cmdline->cmd_op[i + 1][0], "|") == 0)
			cmdline->cmd_io[PIPE_WRITE] = cmdline->pipes[PIPE_RIGHT][PIPE_WRITE];
		if (get_redirect(cmdline, i))
			break ;
		execute_command(find_command_path(cmdline->cmd_op[i][0]), cmdline->cmd_op[i], \
		cmdline->cmd_io[PIPE_READ], cmdline->cmd_io[PIPE_WRITE]);
	}
	close(cmdline->pipes[PIPE_RIGHT][PIPE_READ]); 
	if (pipes_passed > 0)
		close(cmdline->pipes[PIPE_LEFT][PIPE_WRITE]);
}*/
// 50% struct pointers by weight
