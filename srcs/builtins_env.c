/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 10:53:37 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/07 10:58:55 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*store_envvars(char **envp)
{
	t_list	*envvar_list;
	int		i;
	t_env	*envvars;
	t_list	*new_node;

	envvar_list = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		envvars = (t_env *)malloc(sizeof(t_env));
		envvars->cur_envvar = ft_strdup(envp[i]);
		envvars->cur_key = strtok(envvars->cur_envvar, "=");
		envvars->cur_value = strtok(NULL, "=");
		new_node = ft_lstnew(envvars);
		ft_lstadd_front(&envvar_list, new_node);
		i++;
	}
	return (envvar_list);
}
// I'm pretty much stealing the envp variables from the int main

void	free_envvar(t_env *envvar)
{
	free(envvar->cur_envvar);
	free(envvar);
}

void	free_envvar_list(t_list *envvar_list)
{
	t_list	*current;
	t_list	*temp;
	t_env	*envvar;

	current = envvar_list;
	while (current)
	{
		temp = current;
		current = current->next;
		envvar = (t_env *)temp->data;
		free_envvar(envvar);
		free(temp);
	}
}

int	ft_env(t_list *envvar_list)
{
	t_list	*current;
	t_env	*envvar;

	printf("envvar pointer ft_env%p\n", envvar_list);
	current = envvar_list;
	while (current)
	{
		envvar = (t_env *)current->data;
		printf("%s=%s\n", envvar->cur_key, envvar->cur_value);
		current = current->next;
	}
	return (0);
}
// Prints the list of environment variables. 
// This is one of the only builtins that works perfectly right now T_T
