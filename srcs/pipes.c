#include "minishell.h"

//opens a new right pipe
int	update_pipes(t_mini *cmdline, int pipes_passed)
{
	if (pipe(cmdline->pipes[PIPE_RIGHT]) == -1) //open new pipe
	{
		perror("Pipe failed");
		exit(1);
	}
	//printf("opening: %d\nfd read: %d fd write: %d\n", PIPE_RIGHT, cmdline->pipes[PIPE_RIGHT][PIPE_READ], cmdline->pipes[PIPE_RIGHT][PIPE_WRITE]);
	return (pipes_passed);
}

/* It's called handle pipes but really its handle pipes, handle quotes, handle expansions and run the command line.
   Loops through the mess created in cmd_op_tab to set pipe in and pipe out for execute_command().
   Tested with "ping google.com -c 5 | grep rtt | wc | cat -e".
*/
void	handle_pipes(t_mini *cmdline, char *input)
{
	char 	***cmd_op;
	int		i;
	int		pipes_passed;
	int		cmd_io[2];

	i = -1;
	pipes_passed = update_pipes(cmdline, 0); //return value is used to save 2 lines
	cmd_op = cmd_op_tab(input); //input parsing and expansions done here
	while (cmd_op[++i])
	{
		if (ft_strcmp(cmd_op[i][0], "|") == 0)
		{
			pipes_passed = update_pipes(cmdline, pipes_passed + 1);
			continue ;
		}
		cmd_io[PIPE_READ] = -1;
		cmd_io[PIPE_WRITE] = -1;
		//printf("\npipe_right: read %d write %d\n", cmdline->pipes[PIPE_RIGHT][PIPE_READ], cmdline->pipes[PIPE_RIGHT][PIPE_WRITE]);
		//if (pipes_passed > 0)
		//	printf("\npipe_left: read %d write %d\n", cmdline->pipes[PIPE_LEFT][PIPE_READ], cmdline->pipes[PIPE_LEFT][PIPE_WRITE]);
		if (i > 0 && pipes_passed > 0 && ft_strcmp(cmd_op[i - 1][0], "|") == 0)
			cmd_io[PIPE_READ] = cmdline->pipes[PIPE_LEFT][PIPE_READ];
		if (cmd_op[i + 1] && ft_strcmp(cmd_op[i + 1][0], "|") == 0)
			cmd_io[PIPE_WRITE] = cmdline->pipes[PIPE_RIGHT][PIPE_WRITE];
		get_redirect(cmd_op, i);
		execute_command(find_command_path(cmd_op[i][0]), cmd_op[i], cmd_io[PIPE_READ], cmd_io[PIPE_WRITE]);
	}
	close(cmdline->pipes[PIPE_RIGHT][PIPE_READ]); 
	if (pipes_passed > 0)
		close(cmdline->pipes[PIPE_LEFT][PIPE_WRITE]);
}