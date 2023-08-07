/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 14:15:30 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/07 11:15:26 by kbrice           ###   ########.fr       */
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
	free(current_dir);
	free(username);
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

int	execute_command(t_cmd *cmd)
{
	pid_t	child_pid;
	int		status;
	char	*env[1];

	env[0] = NULL;
	if (cmd->path == NULL)
		return (command_not_found(cmd->name));
	if (is_directory(cmd->path))
		return (command_is_directory(cmd->path));
	child_pid = fork();
	if (child_pid < 0)
		perror_exit("fork failed", 1);
	if (child_pid == 0)
	{
		if (cmd->fd_in != -1)
			dup2(cmd->fd_in, STDIN_FILENO);
		if (cmd->fd_out != -1)
			dup2(cmd->fd_out, STDOUT_FILENO);
		execve(cmd->path, cmd->argv, env);
		display_errno(cmd->path);
	}
	close(cmd->fd_out);
	waitpid(child_pid, &status, WUNTRACED);
	close(cmd->fd_in);
	return (status);
}

int	main(int argc, char **argv, char **envp)
{
	t_list	*envvar_list;
	char	*input;
	char	*temp;
	t_mini	*cmdline;

	envvar_list = store_envvars(envp);
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
		process(cmdline, envvar_list);
		del_cmdline(cmdline);
		free(input);
	}
	return (0);
}
// Over 25 lines