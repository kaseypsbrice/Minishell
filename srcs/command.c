/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 10:54:12 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/07 10:54:16 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	Iterates through token list and adds each token 
	with type argument to new list	*/
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

/*	Iterates through token list and adds each token 
	with an operator type to new list	*/
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
/*	These two functions^ have literally a single character difference lol
	I could easily make them one function but this is more readable	*/

/*	Creates a cmd from the tokens list and advances it to the next command	*/
t_cmd	*get_command(t_list	**toks)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		perror_exit("cmd malloc failed", 1);
	cmd->args = get_args((*toks)->next);
	cmd->redirs = get_redirs((*toks)->next);
	cmd->builtin = 0;
	if (is_builtin(((t_tok *)(*toks)->data)->str))
		cmd->builtin = 1;
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
/*	Yeah we out here (((t_tok *)(*toks)->data)->str)'ing
	Pointer to the list is passed in to allow it to be advanced
	with the while loop at the end	
	Separating args and redirs into different lists really isn't
	necessary but it makes future fuctions a tad less complex	*/

/*	Scans through tokens to generate a list of t_cmd*s */
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
/*	cur_tok is advanced in get_command*/

/*	Validates the input string and returns a new command line	*/
t_mini	*new_cmdline(char *str, t_list *envvar_list)
{
	t_mini	*cmdline;

	if (validate_input(str))
		return (NULL);
	cmdline = (t_mini *)malloc(sizeof(t_mini));
	if (!cmdline)
		perror_exit("cmdline malloc failed", 1);
	cmdline->toks = get_tokens(str, envvar_list);
	cmdline->cmds = get_commands(cmdline->toks);
	return (cmdline);
}
/*	Could have put this on the stack but all my homies hate the stack	*/
