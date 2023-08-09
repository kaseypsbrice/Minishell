#include "minishell.h"

void	del_env(void *ptr)
{
	t_env	*env;

	env = (t_env *)ptr;
	free(env->cur_envvar);
	free(env->cur_key);
	free(env->cur_value);
	free(ptr);
}