#include "minishell.h"

void	print_prompt(void)
{
	char		cwd[BUFF]; // Array to store the working directory path name
	char		path_delim; // Will be used to get everything past the last occurance of /
	char		*current_dir; // Used to store and return the directory name
	char		*username;

	username = ft_strjoin(getenv("USER"), " ");
	path_delim = '/';
	getcwd(cwd, sizeof(cwd));
	current_dir = ft_strrchr(cwd, path_delim); // Separates the path from the last occurance of '/' onwards
	current_dir++;
	if (ft_strcmp(cwd, getenv("HOME")) == 0)
	{
		current_dir = "~";
	}
	current_dir = ft_strjoin(username, current_dir);
	printf("\033[1;32m%s $ \033[0m", current_dir); // Displays the current directory path.
}

char	*find_command(char *cmd)
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
		{
			return dir;
		}
		free(dir);
		path = strtok(NULL, ":");
	}
	return (NULL);
}

char	**get_input(char *input)
{
	char	**token;
	char	*delim;
	char	*parsed;
	int		index;

	token = malloc(8 * sizeof(char *));
	if (token == NULL)
	{
		perror("malloc failed");
		exit(1);
	}
	delim = " ";
	index = 0;
	parsed = strtok(input, delim); // Splits the string using the delim and returns an array of strings.
	while (parsed != NULL)
	{
		token[index] = parsed;
		index++;
		parsed = strtok(NULL, delim);
	}
	token[index] = NULL; // Marks the end of the array
	return (token); // Returns the command array which contains the tokens extracted from the input string.
}

void	execute_command(char *command_path, char **command, char *temp, int pipe_in, int pipe_out)
{
	pid_t	child_pid;
	int		status;
	char	*env[1]; // Size of 1

	temp = NULL;
	env[0] = NULL; // Assigns null to the first and only element of the array
	if (command_path == NULL)
	{
		perror("Command path not found"); // ------------ Replace with CMD_NOT_FOUND macro -----------
		return ;
	}
	child_pid = fork();
	if (child_pid < 0)
	{
		perror("fork failed");
		free(temp);
		exit(1);
	}
	if (child_pid == 0)
	{
		if (pipe_in != -1)
		{
			dup2(pipe_in, STDIN_FILENO); // Sets pipe_in as the input for the command
			close(pipe_in);
		}
		if (pipe_out != -1) // Handling output redirection
		{
			dup2(pipe_out, STDOUT_FILENO);
			close(pipe_out);
		}
		execve(command_path, command, env);
		perror("execve failed");
		exit(1);
	}
	else
	{
		waitpid(child_pid, &status, WUNTRACED);
	}
}

int main(int argc, char **argv)
{
	t_mini cmdline;
	char *input;
	char *temp;

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
			continue;
		}
		handle_pipes(cmdline, input);
		free_io(input, cmdline.command);
	}
	return 0;
}