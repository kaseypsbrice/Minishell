#include "minishell.h"

// NOTE: These functions are all WIP and don't currently function correctly

/* Attaches remaining arguments after redirect 
   'echo test > file.txt 123' outputs 'test 123' into file.txt
*/
char	**attach_args(char **dest, char **src)
{
	int		s_len;
	int		d_len;
	int		i;
	char	**temp;

	s_len = -1;
	d_len = -1;
	temp = dest;
	while (src[++s_len]);
	while (dest[++d_len]);
	dest = (char **)malloc((s_len + d_len + 1) * sizeof(char *));
	if (!dest)
	{
		perror("malloc failed");
		exit(1);
	}
	i = -1;
	while (++i < d_len)
		dest[i] = temp[i];
	/*i--;
	while (++i < d_len + s_len)
		dest[i] = src[i - d_len];*/
	dest[i] = NULL;
	free(temp);
	return (dest);
}

void	redirect_out(char ***cmd_op, int *index, int flags)
{
	int	fd;

	printf("redirecting output\n");

	if (!cmd_op[*index + 2])
	{
		ft_putstr_fd("Minishell: syntax error near unexpected token 'newline'\n", 1);
		exit(1);
	}
	fd = open(cmd_op[*index + 2][0], flags);
	if (fd == -1)
	{
		ft_putstr_fd("Minishell: could not open file: ", 1);
		ft_putstr_fd(cmd_op[*index + 2][0], 1);
		ft_putstr_fd("\n", 1);
		exit(1);
	}
	if (cmd_op[*index + 2][1])
		cmd_op[*index] = attach_args(cmd_op[*index], &(cmd_op[*index + 2][1]));
	print_2d(cmd_op[*index]);
}

void	get_redirect(char ***cmd_op, int	index)
{
	if (!cmd_op[index + 1])
		return ;
	/*if (ft_strcmp(tokens[index], "<"))
		return (R_IN);
	if (ft_strcmp(tokens[index], ">"))
		return (R_OUT);
	if (ft_strcmp(tokens[index], ">>"))
		return (R_OUTA);
	if (ft_strcmp(tokens[index], "<<"))
		return (R_DELIM);
	return (R_NONE);*/
	if (ft_strcmp(cmd_op[index + 1][0], ">") == 0)
		redirect_out(cmd_op, &index, O_WRONLY & O_CREAT);
}