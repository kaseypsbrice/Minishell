#include "minishell.h"

void	del_tok(void *ptr)
{
	free(((t_tok *)ptr)->str);
	free(ptr);
}