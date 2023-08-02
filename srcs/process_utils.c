#include "minishell.h"

// Opens a new right pipe
int	update_pipes(t_mini *cmdline, int i)
{
	if (pipe(cmdline->pipes[i % 1]) == -1)
	{
		perror("Pipe failed");
		exit(1);
	}
	return (i);
}
/*	printf("opening: %d\nfd read: 
%d fd write: %d\n", i % 1, \
->pipes[i % 1][PIPE_READ], 
cmdline->pipes[i % 1][PIPE_WRITE]);	*/

//	Opens the pipes in cmdline
int	init_pipes(t_mini *cmdline)
{
	if (pipe(cmdline->pipes[0]) == -1 || pipe(cmdline->pipes[1]) == -1)
	{
		perror("Pipe failed");
		exit(1);
	}
	return (0);
}

/*	Shoehorned function to save lines, expands environment
	variables and then removes quotes	*/
void	expansions_quotes(char **str)
{
	*str = do_expansions(*str);
	remove_quotes(*str);
}