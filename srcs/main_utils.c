#include "minishell.h"

/* Recoding builtin checker */
int	exec_builtins(/*t_cmd *cmd, */char **command)
{
	if (!ft_strcmp(command[0], "cd"))
		ft_cd(command[1]);
	/*else if (!ft_strcmp(command[0], "$?"))
		ft_exit_status();*/
	else if (!ft_strcmp(command[0], "pwd"))
		ft_pwd();
	/*else if (!ft_strcmp(command[0], "exit"))
		ft_exit(command[1]);*/
	/*else if (!ft_strcmp(command[0], "echo"))
		ft_echo(command);*/
	else
		return (0);
	return (1);
}

// returns a token like strtok except it doesnt split spaces if they are quoted
// this and get_input should probably be relocated to a parse.c file or something
char	*get_token(char *input)
{
	int		i;
	char	last_quote;
	char	*res;

	i = -1;
	last_quote = 0;
	while (input[++i])
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			if (!last_quote)
				last_quote = input[i];
			else if (input[i] == last_quote)
				last_quote = 0;
		}
		if (input[i] == ' ' && !last_quote)
			break;
	}
	res = (char *)malloc((i + 1) * sizeof(char));
	if (!res)
	{
		perror("malloc failed");
		exit(1);
	}
	ft_strlcpy(res, input, i + 1);
	res[i] = 0;
	return (res);
}

// removes char at index and decrements index to account for it
void	remove_at(char *str, int *index)
{
	int	len;
	int	i;

	len = ft_strlen(str);
	i = *index - 1;
	while (++i < len)
		str[i] = str[i + 1];
	(*index)--;
}

// removes unclosed quotes
void	rem_unclosed(char *input)
{
	int		unclosed;
	char	last_quote;
	int		i;

	last_quote = 0;
	i = -1;
	while (input[++i])
	{
		if (!last_quote && (input[i] == ';' || input[i] == '\\'))
		{
			remove_at(input, &i);
			continue ;
		}
		if (input[i] == '\'' || input[i] == '\"')
		{
			if (!last_quote)
			{
				last_quote = input[i];
				unclosed = i;
			}
			else if (input[i] == last_quote)
				last_quote = 0;
		}
	}
	if (last_quote)
	{
		remove_at(input, &unclosed);
		rem_unclosed(input); //in case of double unclosed ('")
	}
}

void	free_io(char *input, char **command)
{
	free(input);
	//free(command);
}