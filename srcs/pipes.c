#include "minishell.h"

void handle_pipes(t_mini cmdline, char *input)
{
    char **command1;
    char **command2;
    char *command1_path;
    char *command2_path;
    int pipefd[2];
    int pipe_index;
    int i;

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
        command1 = cmdline.command;
        command2 = cmdline.command + pipe_index + 1;
        command1_path = find_command(command1[0]);
        command2_path = find_command(command2[0]);
        if (pipe(pipefd) == -1)
        {
            perror("pipe failed");
            free_io(input, cmdline.command);
            return;
        }
        execute_command(command1_path, command1, NULL, -1, pipefd[1]);
        close(pipefd[1]);
        execute_command(command2_path, command2, NULL, pipefd[0], -1);
        close(pipefd[0]);
    }
    else
    {
        cmdline.command_path = find_command(cmdline.command[0]);
        execute_command(cmdline.command_path, cmdline.command, NULL, -1, -1);
    }
}