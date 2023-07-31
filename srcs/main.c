/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 14:15:30 by kbrice            #+#    #+#             */
/*   Updated: 2023/07/25 14:01:17 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_prompt(void)
{
	char		cwd[BUFF];
	char		path_delim;
	char		*current_dir;
	char		*username;

	username = ft_strjoin(getenv("USER"), " ");
	path_delim = '/';
	getcwd(cwd, sizeof(cwd));
	current_dir = ft_strrchr(cwd, path_delim);
	current_dir++;
	if (ft_strcmp(cwd, getenv("HOME")) == 0)
	{
		current_dir = "~";
	}
	current_dir = ft_strjoin(username, current_dir);
	ft_putstr_fd("\001\033[1;32m\002", STDOUT_FILENO);
	ft_putstr_fd(current_dir, STDOUT_FILENO);
	ft_putstr_fd(" $ \001\033[0m\002", STDOUT_FILENO);
	//printf("\001\033[1;32m\002%s $ \001\033[0m\002", current_dir);
}
/* Result example:	kbrice gh_minishell $ 
 * 					USERNAME DIRECTORY $ 
 * 
 * Uses getcwd() to get the current working directory 
 * e.g. /Users/kbrice/gh_minishell
 * Uses strrchr to store the ouput from the last occurance of '/' onwards 
 * e.g. /gh_minishell
 * Increments the string to remove the '/' symbol from the current_dir string.
 * If the current directory is the same as the HOME env variable, your root
 * directory, then it displays '~'. Lastly, joins the username and current_dir
 * strings before printing to the ouput using printf.
 */


/* Temporary error function. Shouldn't exit programme but should reset prompt.
 */
void	unexpected_token(char c)
{
	ft_putstr_fd("Minishell: syntax error near unexpected token '", 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("'\n", 2);
	exit(1);
}

int	execute_command(char *command_path, char **command, int pipe_in, int pipe_out)
{
	pid_t	child_pid;
	int		status;
	char	*env[1];

	printf("command path %s\n", command_path);
	env[0] = NULL;
	if (command_path == NULL)
	{
		perror("Command path not found");
		return (1);
	}
	child_pid = fork();
	if (child_pid < 0)
	{
		perror("fork failed");
		exit(1);
	}
	if (child_pid == 0)
	{
		if (pipe_in != -1)
			dup2(pipe_in, STDIN_FILENO);
		if (pipe_out != -1)
			dup2(pipe_out, STDOUT_FILENO);
		execve(command_path, command, env);
		perror("execve failed");
		exit(1);
	}
	close(pipe_out);
	waitpid(child_pid, &status, WUNTRACED);
	close(pipe_in);
	return (status);
}
/* Variable Overview:
 * command_path // Full path to the command
 * command // Array of strings representing the command and its arguments
 * pipe_in // File descriptor for input redirection
 * pipe_out // File descriptor for ouput redirection
 * 
 * NOTE: Not norm compliant. Has over 25 lines.
 * TBC...
 */

int	main(int argc, char **argv)
{
	char	*input;
	char	*temp;
	t_mini	*cmdline;

	temp = NULL;
	(void)argc;
	(void)argv;
	while (1)
	{
		print_prompt();
		run_signals(1);
		input = readline(temp);
		if (!input)
			break ;
		add_history(input);
		if (ft_strcmp(input, "") == 0)
		{
			free(input);
			printf("\n");
			continue ;
		}
		cmdline = new_cmdline(input);
		handle_pipes(cmdline);
		del_cmdline(cmdline);
		free(input);
	}
	return (0);
}
