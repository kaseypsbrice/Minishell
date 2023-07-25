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

char	*find_abs_path(char *cmd)
{
	if (cmd == NULL)
		return (NULL);
	if (cmd[0] == '/')
	{
		if (access(cmd, F_OK) == 0)
			return (strdup(cmd));
		else
			return (NULL);
	}
	return (NULL);
}
/* Checks if the command is an absolute path and whether or not it's accessible.
 * If it is, it returns a copy of the path. Returns NULL if the command doesn't 
 * exist, if it isn't accessible, or if it's not an absolute path.
 */

char	*find_command_path(char *cmd)
{
	char	*absolute_path;
	char	*env_path;
	char	*path;
	char	*dir;

	absolute_path = find_abs_path(cmd);
	if (absolute_path != NULL)
		return (absolute_path);
	env_path = getenv("PATH");
	path = strtok(strdup(env_path), ":");
	while (path)
	{
		dir = ft_strjoin(path, "/");
		dir = ft_strjoin(dir, cmd);
		if ((access(dir, F_OK) == 0) || (access(dir, F_OK) == 0))
			return (dir);
		free(dir);
		path = strtok(NULL, ":");
	}
	return (NULL);
}
/* The PATH environment variable contains a list of directories that can be 
 * searched to find a command. The function goes through each directoy listed
 * in the PATH variable and appends '/' plus the name of the command. If the
 * file exists and is accessible using that path, it returns the path. If the
 * file doesn't exist or is not accessible, it continues moving through the
 * list of directories, testing each one to check for the command. If the
 * command isn't found in any of the directories then the function
 * returns NULL.
 */

/* Temporary error function. Shouldn't exit programme but should reset prompt.
 */
void	unexpected_token(char c)
{
	ft_putstr_fd("Minishell: syntax error near unexpected token '", 1);
	ft_putchar_fd(c, 1);
	ft_putstr_fd("'\n", 1);
	exit(1);
}

/* This function isn't used, I'm just leaving it here for now to 
   remind myself of who I used to be. I am a better person now.
int	is_operator(char *str, int i)
{
	if (str[i] == '>')
	{
		if (str[i + 1] == '>')
		{
			if (str[i + 2] && !is_c_valid(str[i + 2]))
				unexpected_token(str[i + 2]);
			return (2);
		}
		return (1);
	}
	if (str[i] == '<')
	{
		if (str[i + 1] == '<')
		{
			if (str[i + 2] && !is_c_valid(str[i + 2]))
				unexpected_token(str[i + 2]);
			return (2);
		}
		return (1);
	}
	if (str[i] == '|')
	{
		if (str[i + 1] && !is_c_valid(str[i + 1]))
			unexpected_token(str[i + 1]);
		return (1);
	}
	return (0);
}
*/

void	execute_command(char *command_path, char **command, int pipe_in, int pipe_out)
{
	//printf("executing %s\n", command[0]);
	pid_t	child_pid;
	int		status;
	char	*env[1];

	env[0] = NULL;
	if (command_path == NULL)
	{
		perror("Command path not found");
		return ;
	}
	child_pid = fork();
	if (child_pid < 0)
	{
		perror("fork failed");
		//free(command);
		exit(1);
	}
	if (child_pid == 0)
	{
		if (pipe_in != -1)
		{
			//printf("piping from %d\n", pipe_in);
			dup2(pipe_in, STDIN_FILENO); // Sets pipe_in as the input for the command
		}
		if (pipe_out != -1)
		{
			//printf("piping into %d\n", pipe_out);
			dup2(pipe_out, STDOUT_FILENO);
		}
		execve(command_path, command, env);
		perror("execve failed");
		exit(1);
	}
	close(pipe_out);
	waitpid(child_pid, &status, WUNTRACED); 
	close(pipe_in);
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
	t_mini	cmdline;
	char	*input;
	char	*temp;

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
		handle_pipes(&cmdline, input);
		free(input);
	}
	return (0);
}
