/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 14:15:30 by kbrice            #+#    #+#             */
/*   Updated: 2023/07/11 14:20:56 by kbrice           ###   ########.fr       */
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
	printf("\033[1;32m%s $ \033[0m", current_dir);
}
/* Result example:	kbrice gh_minishell $ 
 * 					USERNAME DIRECTORY $ 
 * 
 * Uses getcwd() to get the current working directory e.g. /Users/kbrice/gh_minishell
 * Uses strrchr to store the ouput from the last occurance of '/' onwards e.g. /gh_minishell
 * Increments the string to remove the '/' symbol from the current_dir string.
 * If the current directory is the same as the HOME env variable, your root directory, then it displays '~'.
 * Lastly, joins the username and current_dir strings before printing to the ouput using printf.
 */

char	*find_command_path(char *cmd)
{
	char	*env_path;
	char	*path;
	char	*dir;

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
/* The PATH environment variable contains a list of directories that can be searched to find a command.
 * The function goes through each directoy listed in the PATH variable and appends '/' plus the name of
 * the command. If the file exists and is accessible using that path, it returns the path. If the file
 * doesn't exist or is not accessible, it continues moving through the list of directories, testing
 * each one to check for the command. If the command isn't found in any of the directories then the
 * function returns NULL.
 */


// replaced strtok to handle spaces between quotes
char	**get_input(char *input)
{
	char	**tokens;
	int		i;
	char	last_quote;

	rem_unclosed(input);
	tokens = malloc(1024 * sizeof(char *));
	if (tokens == NULL)
	{
		perror("malloc failed");
		exit(1);
	}
	i = 0;
	while(*input)
	{
		if (*input == ' ')
		{
			input++;
			continue ;
		}
		tokens[i] = get_token(input);
		input += ft_strlen(tokens[i]);
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}

/*char	**get_input(char *input)
{
	char	**token;
	char	*delim;
	char	*parsed;
	int		index;

	token = malloc(1024 * sizeof(char *));
	if (token == NULL)
	{
		perror("malloc failed");
		exit(1);
	}
	delim = " ";
	index = 0;
	parsed = strtok(input, delim);
	while (parsed != NULL)
	{
		token[index] = parsed;
		index++;
		parsed = strtok(NULL, delim);
	}
	token[index] = NULL; // Marks the end of the array
	return (token); // Returns the command array which contains the tokens extracted from the input string.
}*/

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
		free(command);
		exit(1);
	}
	if (child_pid == 0)
	{
		if (pipe_in != -1)
		{
			//printf("piping from %d\n", pipe_in);
			dup2(pipe_in, STDIN_FILENO); // Sets pipe_in as the input for the command
			close(pipe_in); //can be cut to save lines i think (closed on programme exit)

		}
		if (pipe_out != -1)
		{
			//printf("piping into %d\n", pipe_out);
			dup2(pipe_out, STDOUT_FILENO);
			close(pipe_out); //can be cut to save lines i think (closed on programme exit)
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
		run_signals(1);
		print_prompt();
		input = readline(temp);
		if (ft_strcmp(input, "") == 0)
		{
			free(input);
			printf("\n");
			continue ;
		}
		handle_pipes(&cmdline, input);
		free_io(input, cmdline.command);
	}
	return (0);
}
