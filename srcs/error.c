# include "minishell.h"

void	perror_exit(const char *msg, int status)
{
	perror(msg);
	exit(status);
}

int	display_errno(char *filepath)
{
	int	errnum;

	errnum = errno;
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(filepath, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errnum), 2);
	ft_putstr_fd("\n", 2);
	return (1);
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