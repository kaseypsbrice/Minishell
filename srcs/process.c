/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 09:22:09 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/07 11:17:23 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	Attempts to open a file for redirection
	Returns 0 on success
	Failure prints error and returns 1	*/
int	try_open(t_cmd *cmd, t_tok *redir)
{
	int	fd;

	if (redir->type == R_OUTPUT)
		fd = open(redir->str, O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
	else if (redir->type == R_OUTPUT_A)
		fd = open(redir->str, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
	else if (redir->type == R_INPUT)
		fd = open(redir->str, O_RDONLY);
	else
		return (heredoc(cmd, redir->str));
	if (fd == -1)
		return (display_errno(redir->str));
	if (redir->type == R_INPUT)
	{
		close(cmd->fd_in);
		cmd->fd_in = fd;
	}
	else
	{
		close(cmd->fd_out);
		cmd->fd_out = fd;
	}
	return (0);
}

/*	Opens a file for each redirection
	Sets fd_in/out to the last redirection	
	(Behaves according to bash)	*/
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

/*	Advance the cmds list to the last input redirection.
	Input redirection overrides piping.	*/
t_list	*advance_cmds(t_list *cmds)
{
	t_list	*res;
	t_list	*cur_redir;

	res = cmds;
	while (cmds)
	{
		cur_redir = ((t_cmd *)(cmds->data))->redirs;
		while (cur_redir)
		{
			if (((t_tok *)(cur_redir->data))->type == R_INPUT \
			|| ((t_tok *)(cur_redir->data))->type == R_HEREDOC)
				res = cmds;
			cur_redir = cur_redir->next;
		}
		cmds = cmds->next;
	}
	return (res);
}

/*	Formally called handle pipes, runs the command line.	*/
void	process(t_mini *cmdline, t_list *envvar_list) // Also added envp here, changed the header file too.
{
	t_list	*cur;
	t_cmd	*cmd;
	int		i;

	i = init_pipes(cmdline);
	cur = advance_cmds(cmdline->cmds);
	while (cur)
	{
		cmd = (t_cmd *)cur->data;
		if (handle_redirects(cmd))
			return ;
		if (cmd->fd_out == -1 && cur->next)
			cmd->fd_out = cmdline->pipes[i % 2][PIPE_WRITE];
		if (cur->next)
			((t_cmd *)cur->next->data)->fd_in = \
			cmdline->pipes[i % 2][PIPE_READ];
		if (execute_command(cmd, envvar_list))
			return ;
		close(cmdline->pipes[i % 2][PIPE_WRITE]);
		i = update_pipes(cmdline, i + 1);
		cur = cur->next;
	}
	close(cmdline->pipes[i % 2][PIPE_READ]);
	if (i > 0)
		close(cmdline->pipes[(i + 1) % 2][PIPE_WRITE]);
}
/*	Right Pipe = (i % 2), Left Pipe = ((i + 1) % 2)	
	handle_redirects opens the required files for the operators.
	If fd_out was not set by handle_redirects and there is a next command
	then we pipe the output to the next command.
	Regardless of the current command's redirection the next command must have
	it's input defaulted to the same pipe. This means that it will have no input
	in a case such as 'echo "test" > f.txt | cat', this mirrors bash.
	*/
