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

int	g_exit_status = 0;

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

int	_execute_command(t_cmd *cmd, t_list *envvar_list, char **envvar_arr)
{
	pid_t	child_pid;

	child_pid = fork();
	if (child_pid < 0)
		perror_exit("fork failed", 1);
	if (child_pid == 0)
	{
		if (cmd->fd_in != -1)
			dup2(cmd->fd_in, STDIN_FILENO);
		if (cmd->fd_out != -1)
			dup2(cmd->fd_out, STDOUT_FILENO);
		if (cmd->builtin)
			exit(exec_builtins(cmd, envvar_list));
		execve(cmd->path, cmd->argv, envvar_arr);
		display_errno(cmd->path);
		exit (1);
	}
	close(cmd->fd_out);
	waitpid(child_pid, &g_exit_status, WUNTRACED);
	g_exit_status = WEXITSTATUS(g_exit_status);
	close(cmd->fd_in);
	return (g_exit_status);
}

int	execute_command(t_cmd *cmd, t_list *envvar_list)
{
	char	**envvar_arr;
	int		new_status;

	new_status = 0;
	if (!ft_strcmp(cmd->name, "export") || !ft_strcmp(cmd->name, "cd"))
		new_status = exec_builtins(cmd, envvar_list);
	if (!cmd->builtin && (cmd->path == NULL || is_directory(cmd->path)))
		new_status = command_not_found(cmd->name);
	if (new_status != 0)
	{
		g_exit_status = new_status;
		return (g_exit_status);
	}
	envvar_arr = get_env_arr(envvar_list);
	g_exit_status = _execute_command(cmd, envvar_list, envvar_arr);
	del_envvar_arr(envvar_arr);
	return (g_exit_status);
}

void	main_loop(t_list *envvar_list, t_mini *cmdline, char *input, char *temp)
{
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
		if (cmdline)
		{
			process(cmdline, envvar_list);
			del_cmdline(cmdline);
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_list	*envvar_list;
	char	*input;
	char	*temp;
	t_mini	*cmdline;

	envvar_list = store_envvars(envp);
	input = NULL;
	cmdline = NULL;
	temp = NULL;
	(void)argc;
	(void)argv;
	main_loop(envvar_list, cmdline, input, temp);
	return (0);
}
// Over 25 lines