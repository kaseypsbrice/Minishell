#include "minishell.h"

void	del_tok(void *ptr)
{
	free(((t_tok *)ptr)->str);
	free(ptr);
}

void	del_cmd(void *ptr)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)ptr;
	free(cmd->name);
	free(cmd->path);
	free(cmd->argv);
	free(ptr);
}

void	del_cmdline(t_mini *cmdline)
{
	ft_lstclear(&(cmdline->cmds), del_cmd);
	ft_lstclear(&(cmdline->toks), del_tok);
}