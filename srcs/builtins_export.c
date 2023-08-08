/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 10:53:49 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/07 15:19:41 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**split_expvar(char *arg)
{
	char	**temp;

	temp = ft_split(arg, '=');
	return (temp);
}
/* Separates the key and the value of the given
 * export variable.
 */

static int	valid_envvar(char *var)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(var[i]) || var[i] == '_'))
		return (1);
	i++;
	while (var[i] && var[i] != '=')
	{
		if (!(ft_isalnum(var[i]) || var[i] == '_'))
			return (1);
		i++;
	}
	return (0);
}
// Checks the validity of the given export variable

void	set_envvar(t_list **envvar_list, char *key, char *value)
{
	t_list	*current;
	t_env	*new_envvar;
	t_list	*new_node;
	t_env	*envvar;

	current = *envvar_list;
	while (current) 
	{
		envvar = (t_env *)current->data;
		if (strcmp(envvar->cur_key, key) == 0) 
		{
			free(envvar->cur_value);
			envvar->cur_value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	new_envvar = (t_env *)malloc(sizeof(t_env));
	new_envvar->cur_key = ft_strdup(key);
	new_envvar->cur_value = ft_strdup(value);
	new_node = ft_lstnew(new_envvar);
	ft_lstadd_back(envvar_list, new_node);
}
// Iterates through the list to check if the variable exists
// If the variable exists it updates its value and returns
// If the variable doesn't exist then it adds a new one to the list

int	ft_export(char **args, t_list **envvar_list)
{
	int		i;
	char	**temp;

	i = 1;
	if (!args[i])
	{
		ft_env(*envvar_list);
		return (EXIT_SUCCESS);
	}
	printf("Export issue debugging | before args[i] loop\n");
	while (args[i])
	{
		printf("Export issue debugging | In while loop\n");
		if (valid_envvar(args[i]) == 1)
		{
			printf("Export issue debugging | After validity check\n");
			ft_putstr_fd("export : not a valid env variable\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		else if (ft_strchr(args[i], '=') != NULL)
		{
			temp = split_expvar(args[i]);
			set_envvar(envvar_list, temp[0], temp[1]);
			free(temp);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	ft_unset(char **var_name, t_list **envvar_list)
{
    t_list *current;
    t_list *prev;
	t_env *envvar;

	current = *envvar_list;
	prev = NULL;
    while (current)
	{
        envvar = (t_env *)current->data;
        if (strcmp(envvar->cur_key, var_name[1]) == 0)
		{
            if (prev)
                prev->next = current->next;
			else
                *envvar_list = current->next;
            ft_lstdelone(current, free);
            return (EXIT_SUCCESS);
        }
        prev = current;
        current = current->next;
    }
	return (EXIT_FAILURE);
}
// Loops through the list created by store_envvars
// to find the environment variable to remove.
