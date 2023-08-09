/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 14:15:30 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/09 16:37:43 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

const char	*print_prompt(void)
{
	char		cwd[BUFF];
	char		*prompt;
	char		*username;
	char		*tmp;

	username = ft_strjoin(getenv("USER"), " ");
	getcwd(cwd, sizeof(cwd));
	tmp = ft_strdup(cwd);
	prompt = ft_substr(tmp, char_index(tmp, '/') + 1, \
	ft_strlen(tmp) - char_index(tmp, '/') - 1);
	if (ft_strcmp(cwd, getenv("HOME")) == 0)
	{
		prompt = ft_strdup("~");
	}
	prompt = ft_strjoinf(username, prompt, 2);
	prompt = ft_strjoinf("\001\033[1;32m\002", prompt, 1);
	prompt = ft_strjoinf(prompt, " $ \001\033[0m\002", 0);
	free(tmp);
	return (prompt);
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
 * directory, then it displays '~'.
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

	if (!ft_strcmp(cmd->name, "export") || !ft_strcmp(cmd->name, "cd") || \
	!ft_strcmp(cmd->name, "unset") || !ft_strcmp(cmd->name, "exit"))
	{
		g_exit_status = exec_builtins(cmd, envvar_list);
		return (g_exit_status);
	}
	if (!cmd->builtin && (cmd->path == NULL || is_directory(cmd->path)))
	{
		g_exit_status = command_not_found(cmd->name);
		return (g_exit_status);
	}
	envvar_arr = get_env_arr(envvar_list);
	g_exit_status = _execute_command(cmd, envvar_list, envvar_arr);
	del_envvar_arr(envvar_arr);
	return (g_exit_status);
}

void	main_loop(t_list *envvar_list, t_mini *cmdline, char *input)
{
	const char	*prompt;

	while (1)
	{
		prompt = print_prompt();
		run_signals(1);
		input = readline(prompt);
		if (!input)
			break ;
		add_history(input);
		if (is_empty(input))
		{
			free(input);
			continue ;
		}
		cmdline = new_cmdline(input, envvar_list);
		if (cmdline)
		{
			process(cmdline, envvar_list);
			del_cmdline(cmdline);
		}
		free((char *)prompt);
		free(input);
	}
	free((char *)prompt);
}

int	main(int argc, char **argv, char **envp)
{
	t_list	*envvar_list;
	char	*input;
	t_mini	*cmdline;

	envvar_list = store_envvars(envp);
	input = NULL;
	cmdline = NULL;
	(void)argc;
	(void)argv;
	main_loop(envvar_list, cmdline, input);
	free_envvar_list(envvar_list);
	return (0);
}
