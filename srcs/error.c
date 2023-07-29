# include "minishell.h"

void	perror_exit(const char *msg, int status)
{
	perror(msg);
	exit(status);
}

char	*invalid_syntax(char c)
{
	if (c == '\n')
		printf("Minishell: syntax error near \
unexpected token 'newline'\n");
	else
		printf("Minishell: syntax error near \
unexpected token '%c'\n", c);
	return (NULL);
}