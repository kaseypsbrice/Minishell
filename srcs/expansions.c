#include "minishell.h"

/*takes a token eg: {dog$HOME'cat'} and returns {dog/home/alex'cat'}
  as messy as all my functions are sorry :P
  len = length of the environment variable name $HOME = 4
  name is copied out of token and passed into getenv to get "/home/alex"
  it is then frankensteined into a "new" string and returned
  the index is advanced so the do_expansions loop skips to the end dog/home/alex(HERE)'cat'
*/
char	*expand_env(char *token, int *idx)
{
	int		len;
	char	*name;
	char	*var;
	char	*new;

	len = 0;
	while (token[*idx + (++len) + 1] && ft_isalnum(token[*idx + len + 1]));
	name = (char *)malloc((len + 1) * sizeof(char));
	if (!name)
	{
		perror("malloc failed");
		exit(1);
	}
	ft_strlcpy(name, (token + *idx + 1), len + 1);
	var = getenv(name);
	if (!var)
		var = "";
	new = (char *)malloc((ft_strlen(token) + ft_strlen(var) - len) * sizeof(char));
	if (!new)
	{
		perror("malloc failed");
		exit(1);
	}
	ft_strlcpy(new, token, *idx + 1);
	ft_strlcat(new, var, ft_strlen(var) + ft_strlen(token));
	ft_strlcat(new, (token + *idx + len + 1), ft_strlen(var) + ft_strlen(token) + 1);
	free(token);
	free(name);
	*idx += ft_strlen(var) - 1;
	return (new);
}

/* expands environment variables and removes outside quotes
   vars inside '' don't expand but vars inside "" do
   environment variables are terminated by non-alpanumeric chars
*/
void	do_expansions(char **tokens)
{
	int		tok;
	int		i;
	char	last_quote;

	tok = -1;
	last_quote = 0;
	while (tokens[++tok])
	{
		i = -1;
		while (tokens[tok][++i])
		{
			if (tokens[tok][i] == '\'' || tokens[tok][i] == '\"')
			{
				if (!last_quote)
				{
					last_quote = tokens[tok][i];
					remove_at(tokens[tok], &i);
					continue ;
				}
				else if (tokens[tok][i] == last_quote)
				{
					last_quote = 0;
					remove_at(tokens[tok], &i);
					continue ;
				}
			}
			if (tokens[tok][i] == '$' && ft_isalnum(tokens[tok][i + 1]))
				tokens[tok] = expand_env(tokens[tok], &i);
		}
	}
}