#include "minishell.h"

// Oh, look. I'm back from rage quitting.
t_list *store_envvars(char **envp)
{
	t_list	*envvar_list = NULL;
	int		i;

	i = 0;
	while (envp[i] != NULL)
	{
		t_env	*envvars = (t_env*)malloc(sizeof(t_env));
		envvars->cur_envvar = ft_strdup(envp[i]);
		envvars->cur_key = strtok(envvars->cur_envvar, "=");
		envvars->cur_value = strtok(NULL, "=");
		t_list	*new_node = ft_lstnew(envvars);
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
	t_list *current = envvar_list;
	while (current)
	{
		t_list *temp = current;
		current = current->next;
		t_env *envvar = (t_env*)temp->data;
		free_envvar(envvar);
		free(temp);
	}
}

void	ft_env(t_list *envvar_list)
{
	t_list	*current = envvar_list;
	while (current)
	{
		t_env *envvar = (t_env*)current->data;
		printf("%s=%s\n", envvar->cur_key, envvar->cur_value);
		current = current->next;
	}
}
// Prints the list of environment variables. 