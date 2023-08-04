#include "minishell.h"

// Oh, look. I'm back from rage quitting.
t_list *store_envvars(char **envp)
{
	t_list	*envvar_list = NULL;
	int		i;

	i = 0;
	while (envp[i] != NULL)
	{
		t_env	*envvars;
		t_env	*new_envvar;
		envvars->cur_envvar = ft_strdup(envp[i]);
		envvars->cur_key = strtok(envvars->cur_envvar, "=");
		envvars->cur_value = strtok(NULL, "=");
		new_envvar = (t_env *)malloc(sizeof(t_env));
		new_envvar->key = ft_strdup(envvars->cur_key);
		new_envvar->value = ft_strdup(envvars->cur_value);
		t_list	*new_node = ft_lstnew(new_envvar);
		ft_lstadd_front(&envvar_list, new_node);
		free(envvars->cur_envvar);
		i++;
	}
	return (envvar_list);
}
/* ...Is this readable? */

/*
t_list *store_envvars(char **envp)
{
    t_list *envvar_list = NULL;
    for (int i = 0; envp[i] != NULL; i++)
    {
        char *env_var = ft_strdup(envp[i]);
        char *cur_key = strtok(env_var, "=");
        char *cur_value = strtok(NULL, "=");
        t_env *new_envvar = (t_env *)malloc(sizeof(t_env));
        new_envvar->key = ft_strdup(cur_key);
        new_envvar->value = ft_strdup(cur_value);
        t_list *new_node = ft_lstnew(new_envvar);
		ft_lstadd_front(&envvar_list, new_node);
		free(envvar_list);
    }
	return (envvar_list);
}
For loop version, first draft of store_envvars. */

void	free_envvars(t_list *envvar_list)
{
	while (envvar_list)
	{
		t_list *temp = envvar_list;
		envvar_list = envvar_list->next;
		t_env *envvar = (t_env*)temp->data;
		free(envvar->key);
		free(envvar->value);
		free(envvar);
		free(temp);
	}
}