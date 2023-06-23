#include "minishell.h"

/* Recoding builtin checker */
int	exec_builtins(/*t_cmd *cmd, */char **command)
{
	// char	*builtin;
	//
	// builtin = cmd->command[0];
	if (!ft_strcmp(command[0], "cd"))
		ft_cd(command[1]);
	/*else if (!ft_strcmp(command[0], "exit"))
		ft_exit(command[1]);*/
	/*else if (!ft_strcmp(command[0], "echo"))
		ft_echo(command);*/
	else
		return (0);
	return (1);
}

void	free_io(char *input, char **command)
{
	free(input);
	free(command);
}