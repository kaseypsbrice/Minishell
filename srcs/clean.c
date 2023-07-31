#include "minishell.h"

/*	Functions to free memory malloc'd when parsing input
	cmd->args->str, cmd->redirs->str and argv[...] don't need to be freed
	as they point to the same strings as toks	*/
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
	//printf("free name\n");
	free(cmd->name);
	//printf("free path\n");
	if (cmd->path)
		free(cmd->path);
	//printf("free argv\n");
	free(cmd->argv);
	//printf("free args\n");
	cur = cmd->args;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}
	//printf("free redirs\n");
	cur = cmd->redirs;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}
	//printf("free cmd\n");
	free(ptr);
}

void	del_cmdline(t_mini *cmdline)
{
	//printf("free cmds\n");
	ft_lstclear(&(cmdline->cmds), del_cmd);
	//printf("free tokens\n");
	ft_lstclear(&(cmdline->toks), del_tok);
	//printf("free cmdline\n");
	free(cmdline);
}