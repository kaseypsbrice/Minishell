/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 10:54:03 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/07 10:54:06 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	Functions to free memory malloc'd when parsing input
	cmd->args->str, cmd->redirs->str and argv[...] don't need to be freed
	as they point to the same strings as toks	*/
void	del_envvar_arr(char **envvar_arr)
{
	int	i;

	i = -1;
	while (envvar_arr[++i])
		free(envvar_arr[i]);
	free (envvar_arr);
}

void	del_path(char **path)
{
	int	i;

	i = 0;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
}

void	del_tok(void *ptr)
{
	free(((t_tok *)ptr)->str);
	free(ptr);
}

void	del_cmd(void *ptr)
{
	t_cmd	*cmd;
	t_list	*cur;
	t_list	*tmp;

	cmd = (t_cmd *)ptr;
	free(cmd->name);
	if (cmd->path)
		free(cmd->path);
	free(cmd->argv);
	cur = cmd->args;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}
	cur = cmd->redirs;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}
	free(ptr);
}

void	del_cmdline(t_mini *cmdline)
{
	ft_lstclear(&(cmdline->cmds), del_cmd);
	ft_lstclear(&(cmdline->toks), del_tok);
	free(cmdline);
}
