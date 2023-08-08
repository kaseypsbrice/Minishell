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

char	**get_env_arr(t_list *envvar_list)
{
	char	**arr;
	char	*new;
	t_list	*cur;
	t_env	*cur_env;
	int		i;

	arr = (char **)malloc(1000 * sizeof(char *));
	cur = envvar_list;
	i = 0;
	while (cur)
	{
		cur_env = (t_env *)cur->data;
		new = ft_strjoin(cur_env->cur_key, "=");
		new = ft_strjoinf(new, cur_env->cur_value, 0);
		arr[i] = new;
		i++;
		cur = cur->next;
	}
	arr[i] = NULL;
	return (arr);
}

t_list	*store_envvars(char **envp)
{
	t_list	*envvar_list;
	int		i;
	t_env	*envvars;
	t_list	*new_node;
	char	**split;

	envvar_list = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		envvars = (t_env *)malloc(sizeof(t_env));
		envvars->cur_envvar = ft_strdup(envp[i]);
		split = ft_split(envvars->cur_envvar, '=');
		envvars->cur_key = split[0];
		envvars->cur_value = split[1];
		free(split);
		new_node = ft_lstnew(envvars);
		ft_lstadd_front(&envvar_list, new_node);
		i++;
	}
	return (envvar_list);
}
// I'm pretty much stealing the envp variables from the int main
// Replaced strtok with ft_split because strtok does't malloc
// it just returns a pointer to a different character of the string
// This causes crashes when trying to export vars stolen from main
// because you're trying to free a pointer that wasn't malloc'd

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
