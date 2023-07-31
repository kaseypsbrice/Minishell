#include "minishell.h"

// Unused
int	has_redir(t_cmd *cmd, int redir)
{
	t_list	*cur;

	cur = cmd->redirs;
	while (cur)
	{
		if (((t_tok *)(cur->data))->type == redir)
			return (1);
		cur = cur->next;
	}
	return (0);
}

/*	Creates char **argv to be passed into execve	*/
char	**assemble_command(t_cmd *cmd)
{
	int		i;
	t_list	*cur;
	char	**res;

	i = 1;
	cur = cmd->args;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	res = (char **)malloc((i + 1) * sizeof(char *));
	if (!res)
		perror_exit("ass_cmd malloc failed", 1);
	res[0] = cmd->name;
	res[i] = 0;
	i = 1;
	cur = cmd->args;
	while (cur)
	{
		res[i++] = ((t_tok *)cur->data)->str;
		cur = cur->next;
	}
	return (res);
}