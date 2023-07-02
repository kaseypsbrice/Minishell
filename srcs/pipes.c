#include "minishell.h"
/*
void handle_pipes(t_mini cmdline, char *input)
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
}
*/

