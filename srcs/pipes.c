#include "minishell.h"
// TODO:
// separate ; from commands to maintain cmd|op|cmd|op form and reset pipes
// and do all the other operators :)))

// these functions are such a mess but it returns an array like: command > operator > command > operator > command
// eg: [("echo", "test"), ("|"), ("cat", "-e")] probably deserves its own struct
// currently it only checks for pipes as operators
void cmd_op_tab_iter(char ***tab, char **raw_tab)
{
	int	i;
	int	j;
	int k;

	i = 0;
	j = 0;
	k = 0;
	tab[j] = (char **)malloc(1024 * sizeof(char*)); // arbitrary buffer size
	while (raw_tab[i])
	{
		if (ft_strcmp(raw_tab[i], "|") == 0)
		{
			tab[j][k] = NULL;
			tab[j + 1] = (char **)malloc(2 * sizeof(char*));
			tab[j + 1][0] = ft_strdup(raw_tab[i]);
			tab[j + 1][1] = NULL;
			j += 2;
			tab[j] = (char **)malloc(1024 * sizeof(char*)); // arbitrary buffer size
			k = 0;
		}
		else {
			tab[j][k] = ft_strdup(raw_tab[i]);
			k++;
		}
		i++;
	}
	tab[j][k] = NULL;
	tab[j + 1] = NULL;
}

char ***cmd_op_tab(char *input)
{
	char 	***tab;
	char	**raw_tab;

	tab = (char ***)malloc(1024 * sizeof(char**)); // arbitrary buffer size
	raw_tab = get_input(input);
	cmd_op_tab_iter(tab, raw_tab);
	return (tab);
}

//opens a new right pipe
int update_pipes(t_mini *cmdline, int pipes_passed)
{
	if (pipe(cmdline->pipes[PIPE_RIGHT]) == -1) //open new pipe
	{
		perror("Pipe failed");
		exit(1);
	}
	//printf("opening: %d\nfd read: %d fd write: %d\n", PIPE_RIGHT, cmdline->pipes[PIPE_RIGHT][PIPE_READ], cmdline->pipes[PIPE_RIGHT][PIPE_WRITE]);
	return (pipes_passed);
}

// loops through the mess created in cmd_op_tab to set pipe in and pipe out for execute_command()
// tested with "ping google.com -c 5 | grep rtt | wc | cat -e"
void handle_pipes(t_mini *cmdline, char *input)
{
	char 	***cmd_op;
	int		i;
	int		pipes_passed;
	int		cmd_io[2];

	i = -1;
	pipes_passed = update_pipes(cmdline, 0); //return value is used to save 2 lines
	cmd_op = cmd_op_tab(input);
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
		if (i > 0 && ft_strcmp(cmd_op[i - 1][0], "|") == 0)
			cmd_io[PIPE_READ] = cmdline->pipes[PIPE_LEFT][PIPE_READ];
		if (cmd_op[i + 1] && ft_strcmp(cmd_op[i + 1][0], "|") == 0)
			cmd_io[PIPE_WRITE] = cmdline->pipes[PIPE_RIGHT][PIPE_WRITE];
		execute_command(find_command(cmd_op[i][0]), cmd_op[i], NULL, cmd_io[PIPE_READ], cmd_io[PIPE_WRITE]);
	}
	close(cmdline->pipes[PIPE_RIGHT][0]); //this might be the only necessary close here
	close(cmdline->pipes[PIPE_LEFT][0]);
	close(cmdline->pipes[PIPE_RIGHT][1]);
	close(cmdline->pipes[PIPE_LEFT][1]);
}

/*void handle_pipes(t_mini cmdline, char *input)
{
    t_pipe  sub;
    int     pipefd[2];
    int     pipe_index;
    int     i;
	
    pipe_index = -1;
    i = 0;
    cmdline.command = get_input(input);
    while (cmdline.command[i] != NULL)
    {
        if (ft_strcmp(cmdline.command[i], "|") == 0)
        {
            pipe_index = i;
            break;
        }
        i++;
    }
    if (pipe_index != -1)
    {
        cmdline.command[pipe_index] = NULL;
        sub.cmd1 = cmdline.command;
        sub.cmd2 = cmdline.command + pipe_index + 1;
        sub.cmd1_path = find_command(sub.cmd1[0]);
        sub.cmd2_path = find_command(sub.cmd2[0]);
        if (pipe(pipefd) == -1)
        {
            perror("pipe failed");
            free_io(input, cmdline.command);
            return;
        }
        execute_command(sub.cmd1_path, sub.cmd1, NULL, -1, pipefd[1]);
        close(pipefd[1]);
        execute_command(sub.cmd2_path, sub.cmd2, NULL, pipefd[0], -1);
        close(pipefd[0]);
    }
    else
    {
        cmdline.command_path = find_command(cmdline.command[0]);
        if (exec_builtins(cmdline.command))
			return ;
        execute_command(cmdline.command_path, cmdline.command, NULL, -1, -1);
    }
}*/
/* More of an experiment than anything. */