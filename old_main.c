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
	printf("%s $ ", current_dir); // Displays the current directory path.
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
		perror("Command path not found");
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
			dup2(pipe_in, STDIN_FILENO); // Set pipe_in as the input for the command
			close(pipe_in);
		}
		if (pipe_out != -1)
		{
			dup2(pipe_out, STDOUT_FILENO);
			close(pipe_out);
		}
		/* Handle pipe output redirection if necessary
		if (_condition for pipe output redirection_)
		{
			// Open file for writing and redirect command output to the file
		}
		*/
		execve(command_path, command, env);
		perror("execve failed");
		exit(1);
	}
	else
	{
		waitpid(child_pid, &status, WUNTRACED);
	}
}

int main(int argc,  char **argv)
{
	//char	**command;
	//char	*command_path;
	t_mini	cmdline;
	char	*input;
	char	*temp;
	int		pipe_index; // ------ + ------- NEW: Used for checking pipe symbol

	temp = NULL;
	(void)argc;
	(void)argv;
	while (1)
	{
		run_signals(1); // -------------- + ---------------- NEW: Signals
		print_prompt();
		input = readline(temp);
		if (ft_strcmp(input, "") == 0)
		{
        	free(input); // Frees input to prevent seg fault & memory leaks
			printf("\n"); // Prints new line to display fresh prompt command line (not sure whether we should be doing this)
        	continue;
		}
		cmdline.command = get_input(input);

		pipe_index = -1; // --- + Pipes
		for (int i = 0; cmdline.command[i] != NULL; i++)
		{
			if (ft_strcmp(cmdline.command[i], "|") == 0)
			{
				pipe_index = i;
				break ;
			}
		}// --- + NEW

		if (pipe_index != -1)
		{
			// Handle commands connected by pipes
			// Split the command array into two separate command arrays
			cmdline.command[pipe_index] = NULL;
			char** command1 = cmdline.command;
			char** command2 = cmdline.command + pipe_index + 1;

			// Get command paths for both commands
			char* command1_path = find_command(command1[0]);
			char* command2_path = find_command(command2[0]);

			// Create a pipe
			int pipefd[2];
			if (pipe(pipefd) == -1)
			{
				perror("pipe failed");
				free_io(input, cmdline.command);
				continue;
			}
			// Execute command1 with pipe_out as input
			execute_command(command1_path, command1, temp, -1, pipefd[1]);
			close(pipefd[1]); // Close unused pipe end
			wait(NULL);
			// Execute command2 with pipe_in as input
			execute_command(command2_path, command2, temp, pipefd[0], -1);
			// Close the pipe
			close(pipefd[0]);
		}
		else
		{
			cmdline.command_path = find_command(cmdline.command[0]);
			if (exec_builtins(cmdline.command))
			{
				free_io(input, cmdline.command);
				continue ;
			}
			execute_command(cmdline.command_path, cmdline.command, temp, -1, -1);
		}
		free_io(input, cmdline.command);
	}
	return (0);
}