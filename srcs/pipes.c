#include "minishell.h"

// these functions are such a mess but it returns an array like: command > operator > command > operator > command
// eg: {("echo", "test"), ("|"), ("cat", "-e")} probably deserves its own struct
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

void create_pipes(t_mini *cmdline, char ***cmd_op) // creates a pipe for each "|"
{
	int i;
	int s;

	i = -1;
	s = 0;
	while (cmd_op[++i])
	{
		if (ft_strcmp(cmd_op[i][0], "|") == 0)
			s++;
	}
	cmdline->pipes = (int **)malloc(s * sizeof(int *));
	i = -1;
	while (++i < s)
	{
		cmdline->pipes[i] = (int *)malloc(2 * sizeof(int));
		if(pipe(cmdline->pipes[i]) == -1)
		{
			perror("pipe failed");
            return;
		}
	}
}

// loops through the mess created in cmd_op_tab to set pipe in and pipe out for execute_command()
// tested with "ping google.com -c 5 | grep rtt | wc"
void handle_pipes(t_mini *cmdline, char *input)
{
	char 	***cmd_op;
	int		i;
	int		pipes_passed;
	int		cmd_pipes[2];

	i = -1;
	cmd_pipes[0] = -1; // PIPE_IN macros would be nice
	cmd_pipes[1] = -1; // PIPE_OUT
	pipes_passed = 0;
	cmd_op = cmd_op_tab(input);
	create_pipes(cmdline, cmd_op);
	while (cmd_op[++i])
	{
		if (ft_strcmp(cmd_op[i][0], "|") == 0)
		{
			pipes_passed++;
			continue ;
		}
		if (i > 0 && ft_strcmp(cmd_op[i - 1][0], "|") == 0)
			cmd_pipes[0] = cmdline->pipes[pipes_passed - 1][0];
		if (cmd_op[i + 1] && ft_strcmp(cmd_op[i + 1][0], "|") == 0)
			cmd_pipes[1] = cmdline->pipes[pipes_passed][1];
		execute_command(find_command(cmd_op[i][0]), cmd_op[i], NULL, cmd_pipes[0], cmd_pipes[1]);
		close(cmd_pipes[0]); // not 100% sure on these closes but it seems to work
		close(cmd_pipes[1]);
	}
	// debug: prints the table
	/*char ***tab = cmd_op_tab(input);
	int j = -1;
	while (tab[++j])
	{
		int k = -1;
		while (tab[j][++k])
			printf("%s ", tab[j][k]);
		printf("\n");
	}*/
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