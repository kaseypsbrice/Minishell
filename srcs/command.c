#include "minishell.h"

t_list	*get_args(t_list *toks)
{
	t_list	*args;
	t_list	*cur;

	args = NULL;
	cur = args;
	while (toks && ((t_tok *)(toks->data))->type != COMMAND)
	{
		if (((t_tok *)(toks->data))->type == ARGUMENT)
		{
			if (!args)
			{
				args = ft_lstnew(toks->data);
				cur = args;
			}
			else
			{
				cur->next = ft_lstnew(toks->data);
				cur = cur->next;
			}
		}
		toks = toks->next;
	}
	return (args);
}

t_list	*get_redirs(t_list *toks)
{
	t_list	*redirs;
	t_list	*cur;

	redirs = NULL;
	cur = redirs;
	while (toks && ((t_tok *)(toks->data))->type != COMMAND)
	{
		if (((t_tok *)(toks->data))->type != ARGUMENT)
		{
			if (!redirs)
			{
				redirs = ft_lstnew(toks->data);
				cur = redirs;
			}
			else
			{
				cur->next = ft_lstnew(toks->data);
				cur = cur->next;
			}
		}
		toks = toks->next;
	}
	return (redirs);
}

t_cmd	*get_command(t_list	**toks)
{
	t_cmd	*cmd;
	
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		perror_exit("cmd malloc failed", 1);
	cmd->args = get_args((*toks)->next);
	cmd->redirs = get_redirs((*toks)->next);
	cmd->name = find_command_name(((t_tok *)(*toks)->data)->str);
	cmd->path = find_command_path(((t_tok *)(*toks)->data)->str);
	cmd->fd_in = -1;
	cmd->fd_out = -1;
	cmd->argv = assemble_command(cmd);
	(*toks) = (*toks)->next;
	while (*toks && ((t_tok *)(*toks)->data)->type != COMMAND)
		(*toks) = (*toks)->next;
	return (cmd);
}
/*	Yeah we out here (((t_tok *)(*toks)->data)->str)'ing */

t_list	*get_commands(t_list *toks)
{
	t_list	*cur_tok;
	t_list	*cmds;
	t_list	*cur_cmd;

	cur_tok = toks;
	cmds = ft_lstnew(get_command(&cur_tok));
	cur_cmd = cmds;
	while (cur_tok)
	{
		cur_cmd->next = ft_lstnew(get_command(&cur_tok));
		cur_cmd = cur_cmd->next;
	}
	return (cmds);
}

t_mini	*new_cmdline(char *str)
{
	t_mini	*cmdline;

	if (validate_input(str))
		return (NULL);
	cmdline = (t_mini *)malloc(sizeof(t_mini));
	if (!cmdline)
		perror_exit("cmdline malloc failed", 1);
	cmdline->toks = get_tokens(str);
	cmdline->cmds = get_commands(cmdline->toks);
	return (cmdline);
}