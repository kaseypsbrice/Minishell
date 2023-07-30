/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 08:33:59 by kbrice            #+#    #+#             */
/*   Updated: 2023/07/19 13:32:46 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Debug commands for printing str arrays and str array arrays (cmd_tab)
void	print_2d(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		printf("%s\n", arr[i]);
}

void	print_3d(char ***arr)
{
	int	i;
	int	j;

	printf("printing 3d\n");
	i = -1;
	while (arr[++i])
	{
		j = -1;
		while (arr[i][++j])
		{
			printf("%s ", arr[i][j]);
		}
		printf("\n");
	}
}

void	print_tokens(t_list *lst)
{
	t_list	*cur;

	cur = lst;
	while (cur)
	{
		printf("%d %s\n", ((t_tok *)cur->data)->type, ((t_tok *)cur->data)->str);
		cur = cur->next;
	}
}

void	print_commands(t_list *cmds)
{
	t_list	*cur;
	t_cmd	*cmd;

	cur = cmds;
	printf("\n----Commands----");
	while (cur)
	{
		printf("\n\n");
		cmd = (t_cmd *)(cur->data);
		printf("Command name: %s\n", cmd->name);
		printf("Command path: %s\n", cmd->path);
		printf("-Command arguments-\n");
		print_tokens(cmd->args);
		printf("-Command redirects-\n");
		print_tokens(cmd->redirs);
		cur = cur->next;
	}
}