/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 09:22:09 by kbrice            #+#    #+#             */
/*   Updated: 2023/07/19 13:24:56 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Opens a new right pipe
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

int	init_pipes(t_mini *cmdline)
{
	if (pipe(cmdline->pipes[0]) == -1 || pipe(cmdline->pipes[1]) == -1)
	{
		perror("Pipe failed");
		exit(1);
	}
	return (0);
}

int	get_output(t_mini *cmdline, int idx, int *pipes_passed)
{

}

int	get_input(t_mini *cmdline, int idx, int *pipes_passed)
{

}

/* It's called handle pipes but really its handle pipes, handle quotes, handle expansions and run the command line.
   Loops through the mess created in cmd_op_tab to set pipe in and pipe out for execute_command().
   Tested with "ping google.com -c 5 | grep rtt | wc | cat -e".
*/
void	handle_pipes(t_mini *cmdline, char *input)
{
	if (validate_input(input))
	{
		printf("Invalid\n");
		return ;
	}
	printf("Valid\n");
}

/*void	handle_pipes(t_mini *cmdline, char *input)
{
	int		i;
	int		pipes_passed;

	i = -1;
	pipes_passed = init_pipes(cmdline);
	cmdline->cmd_op = cmd_op_tab(input);
	while (cmdline->cmd_op[++i])
	{
		if (ft_strcmp(cmdline->cmd_op[i][0], "|") == 0)
		{
			pipes_passed = update_pipes(cmdline, pipes_passed + 1);
			continue ;
		}
		cmdline->cmd_io[PIPE_READ] = -1;
		cmdline->cmd_io[PIPE_WRITE] = -1;
		//printf("\npipe_right: read %d write %d\n", cmdline->pipes[PIPE_RIGHT][PIPE_READ], cmdline->pipes[PIPE_RIGHT][PIPE_WRITE]);
		//if (pipes_passed > 0)
		//	printf("\npipe_left: read %d write %d\n", cmdline->pipes[PIPE_LEFT][PIPE_READ], cmdline->pipes[PIPE_LEFT][PIPE_WRITE]);
		if (i > 0 && pipes_passed > 0 && ft_strcmp(cmdline->cmd_op[i - 1][0], "|") == 0)
			cmdline->cmd_io[PIPE_READ] = cmdline->pipes[PIPE_LEFT][PIPE_READ];
		if (cmdline->cmd_op[i + 1] && ft_strcmp(cmdline->cmd_op[i + 1][0], "|") == 0)
			cmdline->cmd_io[PIPE_WRITE] = cmdline->pipes[PIPE_RIGHT][PIPE_WRITE];
		if (get_redirect(cmdline, i))
			break ;
		execute_command(find_command_path(cmdline->cmd_op[i][0]), cmdline->cmd_op[i], \
		cmdline->cmd_io[PIPE_READ], cmdline->cmd_io[PIPE_WRITE]);
	}
	close(cmdline->pipes[PIPE_RIGHT][PIPE_READ]); 
	if (pipes_passed > 0)
		close(cmdline->pipes[PIPE_LEFT][PIPE_WRITE]);
}*/
// 50% struct pointers by weight
